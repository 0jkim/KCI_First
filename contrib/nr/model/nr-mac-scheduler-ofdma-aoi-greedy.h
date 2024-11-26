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

#ifndef NR_MAC_SCHEDULER_OFDMA_AOI_GREEDY_H
#define NR_MAC_SCHEDULER_OFDMA_AOI_GREEDY_H

#include "nr-mac-scheduler-ofdma.h"
#include "nr-mac-scheduler-ue-info-aoi-greedy.h"
#include <ns3/traced-value.h>
#include <vector>
#include <memory>

namespace ns3 {

/**
 * \ingroup scheduler
 * \brief AoI-Greedy Scheduler for OFDMA-based MAC layer
 *
 * This scheduler is designed to minimize the Age of Information (AoI) for each
 * UE, prioritizing those with higher AoI values for resource allocation. The
 * scheduler operates in the OFDMA mode, allowing resource allocation across
 * frequency and time dimensions.
 */
class NrMacSchedulerOfdmaAoiGreedy : public NrMacSchedulerOfdma
{
public:
  /**
   * \brief GetTypeId
   * \return The TypeId of this class
   */
  static TypeId GetTypeId (void);

  /**
   * \brief Constructor
   */
  NrMacSchedulerOfdmaAoiGreedy ();

  /**
   * \brief Destructor
   */
  virtual ~NrMacSchedulerOfdmaAoiGreedy () override;

protected:
  /**
   * \brief Create an UE representation of the type NrMacSchedulerUeInfoAoiGreedy
   * \param params parameters
   * \return NrMacSchedulerUeInfoAoiGreedy instance
   *
   * This method creates and returns a UE representation specific to the AoI-Greedy scheduler,
   * allowing the scheduler to maintain and utilize AoI metrics for scheduling decisions.
   */
  virtual std::shared_ptr<NrMacSchedulerUeInfo> CreateUeRepresentation (
      const NrMacCschedSapProvider::CschedUeConfigReqParameters &params) const override;

  /**
   * \brief Return the comparison function to sort DL UEs according to AoI
   * \return A function pointer to compare UEs based on AoI for DL
   *
   * This function provides a comparison method that helps prioritize UEs with higher AoI values
   * for downlink scheduling, ensuring the most outdated information is prioritized.
   */
  virtual std::function<bool (const NrMacSchedulerNs3::UePtrAndBufferReq &lhs,
                              const NrMacSchedulerNs3::UePtrAndBufferReq &rhs)>
  GetUeCompareDlFn () const override;

  /**
   * \brief Return the comparison function to sort UL UEs according to AoI
   * \return A function pointer to compare UEs based on AoI for UL
   *
   * This function provides a comparison method that helps prioritize UEs with higher AoI values
   * for uplink scheduling, thereby reducing the average AoI in the network.
   */
  virtual std::function<bool (const NrMacSchedulerNs3::UePtrAndBufferReq &lhs,
                              const NrMacSchedulerNs3::UePtrAndBufferReq &rhs)>
  GetUeCompareUlFn () const override;

  /**
   * \brief Update the UE representation after a symbol (DL) has been assigned to it
   * \param ue UE to which a symbol has been assigned
   * \param assigned the amount of resources assigned
   * \param totAssigned the total amount of resources assigned in the slot
   *
   * Update DL metrics by calling NrMacSchedulerUeInfoAoiGreedy::UpdateDlMetric, which ensures
   * that the Age of Information is appropriately updated after resource allocation.
   */
  virtual void AssignedDlResources (const UePtrAndBufferReq &ue, const FTResources &assigned,
                                    const FTResources &totAssigned) const override;

  /**
   * \brief Update the UE representation after a symbol (UL) has been assigned to it
   * \param ue UE to which a symbol has been assigned
   * \param assigned the amount of resources assigned
   * \param totAssigned the total amount of resources assigned in the slot
   *
   * Update UL metrics by calling NrMacSchedulerUeInfoAoiGreedy::UpdateUlMetric, allowing the
   * scheduler to keep track of AoI improvements for uplink transmissions.
   */
  virtual void AssignedUlResources (const UePtrAndBufferReq &ue, const FTResources &assigned,
                                    const FTResources &totAssigned) const override;

  /**
   * \brief Return the UE comparison function to prioritize AoI for resource allocation.
   * This method will be used during UL and DL scheduling to decide which UE should get resources first.
   */
  virtual std::function<bool (const std::shared_ptr<NrMacSchedulerUeInfo> &lhs,
                              const std::shared_ptr<NrMacSchedulerUeInfo> &rhs)>
  GetUePriorityFn () const;

  virtual void NotAssignedUlResources (const UePtrAndBufferReq &ue, const FTResources &notAssigned,
                                       const FTResources &totalAssigned) const override;

  virtual void NotAssignedDlResources (const UePtrAndBufferReq &ue, const FTResources &notAssigned,
                                       const FTResources &totalAssigned) const override;

  virtual void BeforeDlSched (const UePtrAndBufferReq &ue,
                              const FTResources &assignableInIteration) const override;

  virtual void BeforeUlSched (const UePtrAndBufferReq &ue,
                              const FTResources &assignableInIteration) const override;

private:
  /**
   * \brief Helper function to prioritize UEs based on AoI
   * \param ueList List of UEs to prioritize
   *
   * This function sorts the UE list based on the Age of Information (AoI)
   * metric, ensuring UEs with higher AoI values are prioritized for resource
   * allocation.
   */
  void PrioritizeUesBasedOnAoi (
      std::vector<std::shared_ptr<NrMacSchedulerUeInfoAoiGreedy>> &ueList) const;
};
} // namespace ns3

#endif /* NR_MAC_SCHEDULER_OFDMA_AOI_GREEDY_H */
