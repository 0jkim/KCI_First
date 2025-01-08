#include "nr-mac-scheduler-ue-info-aoi-greedy.h"
#include <ns3/log.h>

namespace ns3 {

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
void
NrMacSchedulerUeInfoAoiGreedy::Update_Normalization(uint32_t currentAoi, uint32_t currentWma)
{
  NS_LOG_FUNCTION (this);
  if(max_aoi == 0 || currentAoi >= max_aoi)
  {
    max_aoi = currentAoi;
  }
  if(min_aoi == 100000||currentAoi <= min_aoi)
  {
    min_aoi = currentAoi;
  }
  if(max_wma == 0 || currentWma>= max_wma)
  {
    max_wma = currentWma;
  }
  if(min_wma == 100000||currentWma<=min_wma)
  {
    min_wma = currentWma;
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

  // Aoi + WMA (Normalization)
  double lMetric = 0.5 * ((luePtr->GetAoi() - luePtr->min_aoi) / 
                        (luePtr->max_aoi - luePtr->min_aoi == 0 ? 1 : luePtr->max_aoi - luePtr->min_aoi)) +
                 0.5 * ((luePtr->GetNiceCount() - luePtr->min_wma) / 
                        (luePtr->max_wma - luePtr->min_wma == 0 ? 1 : luePtr->max_wma - luePtr->min_wma));

  double rMetric = 0.5 * ((ruePtr->GetAoi() - ruePtr->min_aoi) / 
                        (ruePtr->max_aoi - ruePtr->min_aoi == 0 ? 1 : ruePtr->max_aoi - ruePtr->min_aoi)) +
                 0.5 * ((ruePtr->GetNiceCount() - ruePtr->min_wma) / 
                        (ruePtr->max_wma - ruePtr->min_wma == 0 ? 1 : ruePtr->max_wma - ruePtr->min_wma));
  
  // Aoi + WMA (no Normalization)
  // double lMetric = 0.5 * luePtr->GetAoi() + luePtr->GetNiceCount();
  // double rMetric = 0.5 * ruePtr->GetAoi() + ruePtr->GetNiceCount();

  // 단일 Aoi용 스케줄러
  // double lMetric = luePtr->GetAoi();
  // double rMetric = ruePtr->GetAoi();

  // Higher metric gets priority
  return (lMetric > rMetric);
}
} // namespace ns3
