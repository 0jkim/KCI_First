/* aoi-tag.h */
#ifndef PACKET_TAGS_H
#define PACKET_TAGS_H

#include "ns3/tag.h"
#include "ns3/nstime.h"
#include "ns3/uinteger.h"
#include <iostream>

namespace ns3 {

// 패킷 생성 시간 태그 클래스
class PacketCreationTimeTag : public Tag
{
public:
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;

  PacketCreationTimeTag ();
  PacketCreationTimeTag (uint64_t time);

  void SetCreationTime (uint64_t time);
  uint64_t GetCreationTime () const;

  void Serialize (TagBuffer i) const override;
  void Deserialize (TagBuffer i) override;
  uint32_t GetSerializedSize () const override;
  void Print (std::ostream &os) const override;

private:
  uint64_t m_creationTime;
};

// UE ID 태그 클래스
class PacketUeIdTag : public Tag
{
public:
  static TypeId GetTypeId ();
  TypeId GetInstanceTypeId () const override;

  PacketUeIdTag ();
  PacketUeIdTag (uint32_t ueId);

  void SetUeId (uint32_t ueId);
  uint32_t GetUeId () const;

  void Serialize (TagBuffer i) const override;
  void Deserialize (TagBuffer i) override;
  uint32_t GetSerializedSize () const override;
  void Print (std::ostream &os) const override;

private:
  uint32_t m_ueId;
};
class PacketTransmissionTimeTag : public Tag
{
public:
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const override;

  PacketTransmissionTimeTag ();
  virtual ~PacketTransmissionTimeTag (); // 가상 소멸자 추가

  virtual void Serialize (TagBuffer i) const override;
  virtual void Deserialize (TagBuffer i) override;
  virtual uint32_t GetSerializedSize () const override;
  virtual void Print (std::ostream &os) const override;

  void SetTransmissionTime (uint64_t transmissionTime);
  uint64_t GetTransmissionTime (void) const;

private:
  uint64_t m_transmissionTime;
};

} // namespace ns3
#endif // PACKET_TAGS_H