/* aoi-tag.cc */

#include "aoi-tag.h"
#include "ns3/log.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("PacketCreationTimeTag");

NS_OBJECT_ENSURE_REGISTERED (PacketCreationTimeTag);
// PacketCreationTimeTag 구현
PacketCreationTimeTag::PacketCreationTimeTag () : m_creationTime (0)
{
}
PacketCreationTimeTag::PacketCreationTimeTag (uint64_t time) : m_creationTime (time)
{
}

void
PacketCreationTimeTag::SetCreationTime (uint64_t time)
{
  m_creationTime = time;
}

uint64_t
PacketCreationTimeTag::GetCreationTime () const
{
  return m_creationTime;
}

void
PacketCreationTimeTag::Serialize (TagBuffer i) const
{
  i.WriteU64 (m_creationTime);
}

void
PacketCreationTimeTag::Deserialize (TagBuffer i)
{
  m_creationTime = i.ReadU64 ();
}

uint32_t
PacketCreationTimeTag::GetSerializedSize () const
{
  return sizeof (uint64_t);
}

void
PacketCreationTimeTag::Print (std::ostream &os) const
{
  os << "CreationTime=" << m_creationTime;
}

TypeId
PacketCreationTimeTag::GetTypeId ()
{
  static TypeId tid =
      TypeId ("PacketCreationTimeTag").SetParent<Tag> ().AddConstructor<PacketCreationTimeTag> ();
  return tid;
}

TypeId
PacketCreationTimeTag::GetInstanceTypeId () const
{
  return GetTypeId ();
}

// PacketUeIdTag 구현
PacketUeIdTag::PacketUeIdTag () : m_ueId (0)
{
}
PacketUeIdTag::PacketUeIdTag (uint32_t ueId) : m_ueId (ueId)
{
}

void
PacketUeIdTag::SetUeId (uint32_t ueId)
{
  m_ueId = ueId;
}

uint32_t
PacketUeIdTag::GetUeId () const
{
  return m_ueId;
}

void
PacketUeIdTag::Serialize (TagBuffer i) const
{
  i.WriteU32 (m_ueId);
}

void
PacketUeIdTag::Deserialize (TagBuffer i)
{
  m_ueId = i.ReadU32 ();
}

uint32_t
PacketUeIdTag::GetSerializedSize () const
{
  return sizeof (uint32_t);
}

void
PacketUeIdTag::Print (std::ostream &os) const
{
  os << "UeId=" << m_ueId;
}

TypeId
PacketUeIdTag::GetTypeId ()
{
  static TypeId tid = TypeId ("PacketUeIdTag").SetParent<Tag> ().AddConstructor<PacketUeIdTag> ();
  return tid;
}

TypeId
PacketUeIdTag::GetInstanceTypeId () const
{
  return GetTypeId ();
}
TypeId
PacketTransmissionTimeTag::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::PacketTransmissionTimeTag")
                          .SetParent<Tag> ()
                          .AddConstructor<PacketTransmissionTimeTag> ();
  return tid;
}

PacketTransmissionTimeTag::PacketTransmissionTimeTag () : m_transmissionTime (0)
{
}

PacketTransmissionTimeTag::~PacketTransmissionTimeTag ()
{
}

TypeId
PacketTransmissionTimeTag::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

void
PacketTransmissionTimeTag::Serialize (TagBuffer i) const
{
  i.WriteU64 (m_transmissionTime);
}

void
PacketTransmissionTimeTag::Deserialize (TagBuffer i)
{
  m_transmissionTime = i.ReadU64 ();
}

uint32_t
PacketTransmissionTimeTag::GetSerializedSize () const
{
  return sizeof (uint64_t);
}

void
PacketTransmissionTimeTag::Print (std::ostream &os) const
{
  os << "TransmissionTime=" << m_transmissionTime;
}

void
PacketTransmissionTimeTag::SetTransmissionTime (uint64_t transmissionTime)
{
  m_transmissionTime = transmissionTime;
}

uint64_t
PacketTransmissionTimeTag::GetTransmissionTime (void) const
{
  return m_transmissionTime;
}

} // namespace ns3