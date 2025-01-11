#include "nr-mac-scheduler-ue-info-aoi-greedy.h"
#include <ns3/log.h>

namespace ns3 {

uint32_t NrMacSchedulerUeInfoAoiGreedy::global_min_aoi = 100000;
uint32_t NrMacSchedulerUeInfoAoiGreedy::global_max_aoi = 0;
uint32_t NrMacSchedulerUeInfoAoiGreedy::global_min_wma = 100000;
uint32_t NrMacSchedulerUeInfoAoiGreedy::global_max_wma = 0;

NS_LOG_COMPONENT_DEFINE ("NrMacSchedulerUeInfoAoiGreedy");

void
NrMacSchedulerUeInfoAoiGreedy::UpdateAoi (uint32_t currentAoi)
{
  NS_LOG_FUNCTION (this << currentAoi);
  m_aoi = currentAoi;
}

void
NrMacSchedulerUeInfoAoiGreedy::IncrementNiceCount (uint32_t WMA)
{
  NS_LOG_FUNCTION (this);
  m_niceCount = WMA;
}
/**
 * 정규화 정상화 해야함
 */
void
NrMacSchedulerUeInfoAoiGreedy::Update_Normalization (uint32_t currentAoi, uint32_t currentWma)
{
  NS_LOG_FUNCTION (this);
  std::cout<<"현재 최대 최소 aoi 및 wma\n"<<"Max AoI : "<<global_max_aoi<<", Min AoI : "<<global_min_aoi;
  std::cout<<"\nMax WMA : "<<global_max_wma<<", Min WMA : "<<global_min_wma<<"\n";
  if (global_max_aoi == 0 || currentAoi > global_max_aoi)
    { 
      std::cout<<"정규화 업데이트 max aoi를 현재 aoi "<<currentAoi<<"\n";
      global_max_aoi = currentAoi;
    }
  if (global_min_aoi == 100000 || currentAoi < global_min_aoi)
    {
      std::cout<<"정규화 업데이트 min aoi를 현재 aoi "<<currentAoi<<"\n";
      global_min_aoi = currentAoi;
    }
  if (global_max_wma == 0 || currentWma > global_max_wma)
    {
      std::cout<<"정규화 업데이트 max wma를 현재 wma "<<currentWma<<"\n";
      global_max_wma = currentWma;
    }
  if (global_min_wma == 100000 || currentWma < global_min_wma)
    {
      std::cout<<"정규화 업데이트 min wma를 현재 wma "<<currentWma<<"\n";
      global_min_wma = currentWma;
    }
}
uint32_t
NrMacSchedulerUeInfoAoiGreedy::GetAoi () const
{
  NS_LOG_FUNCTION (this);
  return m_aoi;
}

uint32_t
NrMacSchedulerUeInfoAoiGreedy::GetNiceCount () const
{
  NS_LOG_FUNCTION (this);
  return m_niceCount;
}

void
NrMacSchedulerUeInfoAoiGreedy::UpdateMetric (uint32_t currentAoi, uint32_t newNiceCount)
{
  NS_LOG_FUNCTION (this << currentAoi << newNiceCount);
  m_aoi = currentAoi;
  m_niceCount = newNiceCount;
  m_metric = m_aoiWeight * m_aoi + m_niceWeight * m_niceCount;
}

double
NrMacSchedulerUeInfoAoiGreedy::CalculateMetric () const
{
  return m_metric;
}

bool
NrMacSchedulerUeInfoAoiGreedy::CompareUeWeightsDl (const NrMacSchedulerNs3::UePtrAndBufferReq &lue,
                                                   const NrMacSchedulerNs3::UePtrAndBufferReq &rue)
{
  auto luePtr = std::dynamic_pointer_cast<NrMacSchedulerUeInfoAoiGreedy> (lue.first);
  auto ruePtr = std::dynamic_pointer_cast<NrMacSchedulerUeInfoAoiGreedy> (rue.first);

  if (!luePtr || !ruePtr)
    {
      NS_ABORT_MSG ("Failed to cast UePtr to NrMacSchedulerUeInfoAoiGreedy");
    }

  double lMetric = luePtr->CalculateMetric ();
  double rMetric = ruePtr->CalculateMetric ();

  // Higher metric gets priority
  return (lMetric > rMetric);
}

bool
NrMacSchedulerUeInfoAoiGreedy::CompareUeWeightsUl (const NrMacSchedulerNs3::UePtrAndBufferReq &lue,
                                                   const NrMacSchedulerNs3::UePtrAndBufferReq &rue)
{
  auto luePtr = dynamic_cast<NrMacSchedulerUeInfoAoiGreedy *> (lue.first.get ());
  auto ruePtr = dynamic_cast<NrMacSchedulerUeInfoAoiGreedy *> (rue.first.get ());

  float x = 0.1;
  std::cout << "가중치 값 " << x << std::endl;
  // Aoi + WMA (Normalization)
  double lMetric = x * ((luePtr->GetAoi() - NrMacSchedulerUeInfoAoiGreedy::global_min_aoi) /
                        (NrMacSchedulerUeInfoAoiGreedy::global_max_aoi - NrMacSchedulerUeInfoAoiGreedy::global_min_aoi == 0 ? 1 : 
                         NrMacSchedulerUeInfoAoiGreedy::global_max_aoi - NrMacSchedulerUeInfoAoiGreedy::global_min_aoi)) +
                 (1-x) * ((luePtr->GetNiceCount() - NrMacSchedulerUeInfoAoiGreedy::global_min_wma) /
                        (NrMacSchedulerUeInfoAoiGreedy::global_max_wma - NrMacSchedulerUeInfoAoiGreedy::global_min_wma == 0 ? 1 : 
                         NrMacSchedulerUeInfoAoiGreedy::global_max_wma - NrMacSchedulerUeInfoAoiGreedy::global_min_wma));

  double rMetric = x * ((ruePtr->GetAoi() - NrMacSchedulerUeInfoAoiGreedy::global_min_aoi) /
                        (NrMacSchedulerUeInfoAoiGreedy::global_max_aoi - NrMacSchedulerUeInfoAoiGreedy::global_min_aoi == 0 ? 1 : 
                         NrMacSchedulerUeInfoAoiGreedy::global_max_aoi - NrMacSchedulerUeInfoAoiGreedy::global_min_aoi)) +
                 (1-x) * ((ruePtr->GetNiceCount() - NrMacSchedulerUeInfoAoiGreedy::global_min_wma) /
                        (NrMacSchedulerUeInfoAoiGreedy::global_max_wma - NrMacSchedulerUeInfoAoiGreedy::global_min_wma == 0 ? 1 : 
                         NrMacSchedulerUeInfoAoiGreedy::global_max_wma - NrMacSchedulerUeInfoAoiGreedy::global_min_wma));

  // Aoi + WMA (no Normalization)
  // double lMetric = x * luePtr->GetAoi () + (1 - x) * luePtr->GetNiceCount ();
  // double rMetric = x * ruePtr->GetAoi () + (1 - x) * ruePtr->GetNiceCount ();

  std::cout << luePtr->GetRnti () << "의 aoi " << luePtr->GetAoi () << " + wma "
            << luePtr->GetNiceCount () << std::endl;
  std::cout << ruePtr->GetRnti () << "의 aoi " << ruePtr->GetAoi () << " + wma "
            << ruePtr->GetNiceCount () << std::endl;

  std::cout << "lmetric 값 " << lMetric << std::endl;
  std::cout << "rmetric 값 " << rMetric << std::endl;

  // 단일 Aoi용 스케줄러
  // double lMetric = luePtr->GetAoi();
  // double rMetric = ruePtr->GetAoi();

  // Higher metric gets priority
  return (lMetric > rMetric);
}
} // namespace ns3
