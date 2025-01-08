/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 *   Copyright (c) 2024 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License version 2 as
 *   published by the Free Software Foundation;
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "nr-mac-scheduler-ofdma-aoi-greedy.h"
#include "ns3/log.h"
#include <algorithm>

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("NrMacSchedulerOfdmaAoiGreedy");
NS_OBJECT_ENSURE_REGISTERED (NrMacSchedulerOfdmaAoiGreedy);

TypeId
NrMacSchedulerOfdmaAoiGreedy::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::NrMacSchedulerOfdmaAoiGreedy")
                          .SetParent<NrMacSchedulerOfdma> ()
                          .AddConstructor<NrMacSchedulerOfdmaAoiGreedy> ();
  return tid;
}

NrMacSchedulerOfdmaAoiGreedy::NrMacSchedulerOfdmaAoiGreedy () : NrMacSchedulerOfdma ()
{
  NS_LOG_FUNCTION (this);
}

NrMacSchedulerOfdmaAoiGreedy::~NrMacSchedulerOfdmaAoiGreedy ()
{
  NS_LOG_FUNCTION (this);
}

std::shared_ptr<NrMacSchedulerUeInfo>
NrMacSchedulerOfdmaAoiGreedy::CreateUeRepresentation (
    const NrMacCschedSapProvider::CschedUeConfigReqParameters &params) const
{
  NS_LOG_FUNCTION (this);
  return std::make_shared<NrMacSchedulerUeInfoAoiGreedy> (
      params.m_rnti, params.m_beamConfId,
      std::bind (&NrMacSchedulerOfdmaAoiGreedy::GetNumRbPerRbg, this));
}
std::function<bool (const NrMacSchedulerNs3::UePtrAndBufferReq &lhs,
                    const NrMacSchedulerNs3::UePtrAndBufferReq &rhs)>
NrMacSchedulerOfdmaAoiGreedy::GetUeCompareDlFn () const
{
  return [] (const NrMacSchedulerNs3::UePtrAndBufferReq &lhs,
             const NrMacSchedulerNs3::UePtrAndBufferReq &rhs) -> bool {
    return std::static_pointer_cast<NrMacSchedulerUeInfoAoiGreedy> (lhs.first)->GetAoi () >
           std::static_pointer_cast<NrMacSchedulerUeInfoAoiGreedy> (rhs.first)->GetAoi ();
  };
}

std::function<bool (const NrMacSchedulerNs3::UePtrAndBufferReq &lhs,
                    const NrMacSchedulerNs3::UePtrAndBufferReq &rhs)>
NrMacSchedulerOfdmaAoiGreedy::GetUeCompareUlFn () const
{
  /* 원래 쓰던 코드 임시 저장*/
  // std::cout << "[aoi-greedy] : GetUeCompareUlFn\n";
  // return [this] (const NrMacSchedulerNs3::UePtrAndBufferReq &lhs,
  //                const NrMacSchedulerNs3::UePtrAndBufferReq &rhs) -> bool {
  //   // 두 UE의 포인터를 NrMacSchedulerUeInfoAoiGreedy로 캐스팅
  //   auto lhsUePtr = std::dynamic_pointer_cast<NrMacSchedulerUeInfoAoiGreedy> (lhs.first);
  //   auto rhsUePtr = std::dynamic_pointer_cast<NrMacSchedulerUeInfoAoiGreedy> (rhs.first);

  //   uint16_t lhs_rnti = lhs.first->GetRnti ();
  //   uint16_t rhs_rnti = rhs.first->GetRnti ();
  //   uint32_t lhs_aoi = this->GetAge (lhs_rnti);
  //   uint32_t rhs_aoi = this->GetAge (rhs_rnti);
  //   uint32_t lhs_nicecount = lhsUePtr->GetNiceCount ();
  //   uint32_t rhs_nicecount = rhsUePtr->GetNiceCount ();

  //   lhsUePtr->UpdateAoi (lhs_aoi);
  //   rhsUePtr->UpdateAoi (rhs_aoi);

  //   if (!lhsUePtr || !rhsUePtr)
  //     {
  //       NS_ABORT_MSG ("Failed to cast UePtr to NrMacSchedulerUeInfoAoiGreedy");
  //     }

  //   // 각 UE의 메트릭을 계산
  //   double lhsMetric = 0.5 * lhs_aoi + 0.5 * lhs_nicecount;
  //   double rhsMetric = 0.5 * rhs_aoi + 0.5 * rhs_nicecount;
  //   // std::cout << "[aoi-scheduler] : lhsmetric = " << lhsMetric << " , rhsmetric = " << rhsMetric
  //   //           << std::endl;
  //   // 더 높은 메트릭 값을 가진 UE가 우선순위를 가집니다.
  //   return (lhsMetric > rhsMetric);
  // };

  return NrMacSchedulerUeInfoAoiGreedy::CompareUeWeightsUl;
}

void
NrMacSchedulerOfdmaAoiGreedy::AssignedDlResources (const UePtrAndBufferReq &ue,
                                                   const FTResources &assigned,
                                                   const FTResources &totAssigned) const
{
  NS_LOG_FUNCTION (this);
  // std::cout << "[aoi-scheduler] : test\n";
  auto ueInfo = std::static_pointer_cast<NrMacSchedulerUeInfoAoiGreedy> (ue.first);
  ueInfo->UpdateDlMetric (m_dlAmc);
}

void
NrMacSchedulerOfdmaAoiGreedy::AssignedUlResources (const UePtrAndBufferReq &ue,
                                                   const FTResources &assigned,
                                                   const FTResources &totAssigned) const
{
  NS_LOG_FUNCTION (this);
  // std::cout << "[ofdma -aoi-greedy]: test\n";
  // Get the UE info object casted to the appropriate type
  auto ueInfo = std::static_pointer_cast<NrMacSchedulerUeInfoAoiGreedy> (ue.first);

  // Use the AMC object to update UL metrics
  // Assuming `m_ulAmc` is accessible and correctly initialized in this scheduler
  ueInfo->UpdateUlMetric (m_ulAmc); // Corrected to pass AMC for TB size calculation
}

std::function<bool (const std::shared_ptr<NrMacSchedulerUeInfo> &lhs,
                    const std::shared_ptr<NrMacSchedulerUeInfo> &rhs)>
NrMacSchedulerOfdmaAoiGreedy::GetUePriorityFn () const
{
  return [] (const std::shared_ptr<NrMacSchedulerUeInfo> &lhs,
             const std::shared_ptr<NrMacSchedulerUeInfo> &rhs) -> bool {
    return std::static_pointer_cast<NrMacSchedulerUeInfoAoiGreedy> (lhs)->GetAoi () >
           std::static_pointer_cast<NrMacSchedulerUeInfoAoiGreedy> (rhs)->GetAoi ();
  };
}

void
NrMacSchedulerOfdmaAoiGreedy::PrioritizeUesBasedOnAoi (
    std::vector<std::shared_ptr<NrMacSchedulerUeInfoAoiGreedy>> &ueList) const
{
  NS_LOG_FUNCTION (this);
  std::sort (ueList.begin (), ueList.end (),
             [] (const std::shared_ptr<NrMacSchedulerUeInfoAoiGreedy> &lhs,
                 const std::shared_ptr<NrMacSchedulerUeInfoAoiGreedy> &rhs) -> bool {
               return lhs->GetAoi () > rhs->GetAoi ();
             });
}
void
NrMacSchedulerOfdmaAoiGreedy::NotAssignedUlResources (const UePtrAndBufferReq &ue,
                                                      const FTResources &notAssigned,
                                                      const FTResources &totalAssigned) const
{
  NS_LOG_FUNCTION (this);
}

void
NrMacSchedulerOfdmaAoiGreedy::NotAssignedDlResources (const UePtrAndBufferReq &ue,
                                                      const FTResources &notAssigned,
                                                      const FTResources &totalAssigned) const
{
  NS_LOG_FUNCTION (this);
}

void
NrMacSchedulerOfdmaAoiGreedy::BeforeDlSched (const UePtrAndBufferReq &ue,
                                             const FTResources &assignableInIteration) const
{
  NS_LOG_FUNCTION (this);
}

void
NrMacSchedulerOfdmaAoiGreedy::BeforeUlSched (const UePtrAndBufferReq &ue,
                                             const FTResources &assignableInIteration) const
{

  NS_LOG_FUNCTION (this);
  auto uePtr = std::dynamic_pointer_cast<NrMacSchedulerUeInfoAoiGreedy> (ue.first);
  uint16_t ue_rnti = ue.first->GetRnti ();
  uint32_t ue_aoi = this->GetAge (ue_rnti);
  uint32_t ue_WMA = this->GetWMA(ue_rnti);
  uePtr->UpdateAoi (ue_aoi);
  uePtr->IncrementNiceCount(ue_WMA);

  /**
   * 정규화를 위한 min-max (aoi, wma) 값 추출
   */
  uePtr->Update_Normalization(ue_aoi, ue_WMA);
}

} // namespace ns3
