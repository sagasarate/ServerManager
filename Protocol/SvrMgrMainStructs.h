#pragma once



//
struct NET_ADAPTER_INFO
{
	UINT			Index;
	CEasyArray<CEasyString>		IPAddressList;
	CEasyString		Name;
	UINT64			SendBytes;
	float			SendFlux;
	UINT64			RecvBytes;
	float			RecvFlux;
	
	
	enum NET_ADAPTER_INFO_MEMBER_IDS
	{
		SST_NET_ADAPTER_INFO_INDEX = 1,
		SST_NET_ADAPTER_INFO_IPADDRESS_LIST = 2,
		SST_NET_ADAPTER_INFO_NAME = 7,
		SST_NET_ADAPTER_INFO_SEND_BYTES = 5,
		SST_NET_ADAPTER_INFO_SEND_FLUX = 3,
		SST_NET_ADAPTER_INFO_RECV_BYTES = 6,
		SST_NET_ADAPTER_INFO_RECV_FLUX = 4,
		
	};
	
	
	NET_ADAPTER_INFO()
	{
		Clear();
	}
	NET_ADAPTER_INFO(const NET_ADAPTER_INFO& Value)
	{
		*this=Value;
	}
	void Clear()
	{
		Index=0;
		IPAddressList.SetTag(_T("StructData"));
		IPAddressList.Clear();
		IPAddressList.Create(16,8);
		Name.Clear();
		SendBytes=0;
		SendFlux=0;
		RecvBytes=0;
		RecvFlux=0;
		
	}
	bool MakePacket(CSmartStruct& Packet) const
	{
		UINT FailCount=0;

		{
			CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_NET_ADAPTER_INFO_INDEX,Index),FailCount);
		}
		
		{
			CSmartStruct& ParentPacket=Packet;
			{
				CSmartArray Packet=ParentPacket.PrepareSubArray();
				Packet.AddArray(IPAddressList);
				if(!ParentPacket.FinishMember(SST_NET_ADAPTER_INFO_IPADDRESS_LIST,Packet.GetDataLen()))
					FailCount++;
			}
		}
		{
			CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_NET_ADAPTER_INFO_NAME,Name),FailCount);
		}
		
		{
			CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_NET_ADAPTER_INFO_SEND_BYTES,SendBytes),FailCount);
		}
		
		{
			CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_NET_ADAPTER_INFO_SEND_FLUX,SendFlux),FailCount);
		}
		
		{
			CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_NET_ADAPTER_INFO_RECV_BYTES,RecvBytes),FailCount);
		}
		
		{
			CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_NET_ADAPTER_INFO_RECV_FLUX,RecvFlux),FailCount);
		}
		
		

		return FailCount==0;
	}
	bool ParsePacket(const CSmartStruct& Packet)
	{
		
		void * Pos=Packet.GetFirstMemberPosition();
		while(Pos)
		{
			WORD MemberID;
			CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
			switch(MemberID)
			{
			case SST_NET_ADAPTER_INFO_INDEX:
				{
					Index=Value;
				}
				break;
			case SST_NET_ADAPTER_INFO_IPADDRESS_LIST:
				{
					IPAddressList.Clear();
					CSmartArray Packet=Value;
					Packet.GetArray(IPAddressList);
				}
				break;
			
			case SST_NET_ADAPTER_INFO_NAME:
				{
					Value.GetString(Name);
				}
				break;
			case SST_NET_ADAPTER_INFO_SEND_BYTES:
				{
					SendBytes=Value;
				}
				break;
			case SST_NET_ADAPTER_INFO_SEND_FLUX:
				{
					SendFlux=Value;
				}
				break;
			case SST_NET_ADAPTER_INFO_RECV_BYTES:
				{
					RecvBytes=Value;
				}
				break;
			case SST_NET_ADAPTER_INFO_RECV_FLUX:
				{
					RecvFlux=Value;
				}
				break;
			
			}
		}
		
		return true;
	}
	UINT GetSmartStructSize() const
	{
		UINT Size=0;
		{
			Size+=CSmartStruct::GetFixMemberSize(sizeof(UINT));
		}
		
		{
			for(size_t i=0;i<IPAddressList.GetCount();i++)
			{
				Size+=CSmartArray::GetStringMemberSize(IPAddressList[i]);
			}
			Size+=CSmartStruct::GetArrayMemberSize(0);
		}
		{
			Size+=CSmartStruct::GetStringMemberSize(Name);
		}
		
		{
			Size+=CSmartStruct::GetFixMemberSize(sizeof(UINT64));
		}
		
		{
			Size+=CSmartStruct::GetFixMemberSize(sizeof(float));
		}
		
		{
			Size+=CSmartStruct::GetFixMemberSize(sizeof(UINT64));
		}
		
		{
			Size+=CSmartStruct::GetFixMemberSize(sizeof(float));
		}
		
		
		return Size;
	}
	void CloneFrom(const NET_ADAPTER_INFO& Value)
	{
		{
			Index=Value.Index;
		}
		{
			IPAddressList=Value.IPAddressList;
		}
		{
			Name=Value.Name;
		}
		{
			SendBytes=Value.SendBytes;
		}
		{
			SendFlux=Value.SendFlux;
		}
		{
			RecvBytes=Value.RecvBytes;
		}
		{
			RecvFlux=Value.RecvFlux;
		}
		
	}
	NET_ADAPTER_INFO& operator=(const NET_ADAPTER_INFO& Value)
	{
		CloneFrom(Value);
		return *this;
	}
	
	
	
	
	
	

};



//
struct NET_ADAPTER_INFO_LIST
{
	CEasyArray<NET_ADAPTER_INFO>	List;
	
	
	enum NET_ADAPTER_INFO_LIST_MEMBER_IDS
	{
		SST_NET_ADAPTER_INFO_LIST_LIST = 1,
		
	};
	
	
	NET_ADAPTER_INFO_LIST()
	{
		Clear();
	}
	NET_ADAPTER_INFO_LIST(const NET_ADAPTER_INFO_LIST& Value)
	{
		*this=Value;
	}
	void Clear()
	{
		List.SetTag(_T("StructData"));
		List.Clear();
		List.Create(16,8);
		
	}
	bool MakePacket(CSmartStruct& Packet) const
	{
		UINT FailCount=0;

		{
			CSmartStruct& ParentPacket=Packet;
			{
				CSmartArray Packet=ParentPacket.PrepareSubArray();
				for(size_t i=0;i<List.GetCount();i++)
				{
					CSmartStruct SubPacket=Packet.PrepareSubStruct();
					if(!List[i].MakePacket(SubPacket))
						FailCount++;
					if(!Packet.FinishMember(SubPacket.GetDataLen()))
						FailCount++;
				}
				if(!ParentPacket.FinishMember(SST_NET_ADAPTER_INFO_LIST_LIST,Packet.GetDataLen()))
					FailCount++;
			}
		}
		

		return FailCount==0;
	}
	bool ParsePacket(const CSmartStruct& Packet)
	{
		
		void * Pos=Packet.GetFirstMemberPosition();
		while(Pos)
		{
			WORD MemberID;
			CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
			switch(MemberID)
			{
			case SST_NET_ADAPTER_INFO_LIST_LIST:
				{
					List.Clear();
					CSmartArray Packet=Value;
					for(CSmartValue Value : Packet)
					{
						NET_ADAPTER_INFO& ArrayElement=*List.AddEmpty();
						ArrayElement.ParsePacket(Value);
						
					}
				}
				break;
			
			
			}
		}
		
		return true;
	}
	UINT GetSmartStructSize() const
	{
		UINT Size=0;
		{
			for(size_t i=0;i<List.GetCount();i++)
			{
				Size+=CSmartArray::GetStructMemberSize(List[i].GetSmartStructSize());
			}
			Size+=CSmartStruct::GetArrayMemberSize(0);
		}
		
		return Size;
	}
	void CloneFrom(const NET_ADAPTER_INFO_LIST& Value)
	{
		{
			List=Value.List;
		}
		
	}
	NET_ADAPTER_INFO_LIST& operator=(const NET_ADAPTER_INFO_LIST& Value)
	{
		CloneFrom(Value);
		return *this;
	}
	
	
	
	
	
	

};



//
struct FILE_INFO
{
	CEasyString		Name;
	UINT			Attribute;
	UINT64			Size;
	UINT64			CreateTime;
	UINT64			LastAccessTime;
	UINT64			LastWriteTime;
	
	
	enum FILE_INFO_MEMBER_IDS
	{
		SST_FILE_INFO_NAME = 1,
		SST_FILE_INFO_ATTRIBUTE = 2,
		SST_FILE_INFO_SIZE = 3,
		SST_FILE_INFO_CREATE_TIME = 4,
		SST_FILE_INFO_LAST_ACCESS_TIME = 5,
		SST_FILE_INFO_LAST_WRITE_TIME = 6,
		
	};
	
	
	FILE_INFO()
	{
		Clear();
	}
	FILE_INFO(const FILE_INFO& Value)
	{
		*this=Value;
	}
	void Clear()
	{
		Name.Clear();
		Attribute=0;
		Size=0;
		CreateTime=0;
		LastAccessTime=0;
		LastWriteTime=0;
		
	}
	bool MakePacket(CSmartStruct& Packet) const
	{
		UINT FailCount=0;

		{
			CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_FILE_INFO_NAME,Name),FailCount);
		}
		
		{
			CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_FILE_INFO_ATTRIBUTE,Attribute),FailCount);
		}
		
		{
			CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_FILE_INFO_SIZE,Size),FailCount);
		}
		
		{
			CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_FILE_INFO_CREATE_TIME,CreateTime),FailCount);
		}
		
		{
			CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_FILE_INFO_LAST_ACCESS_TIME,LastAccessTime),FailCount);
		}
		
		{
			CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_FILE_INFO_LAST_WRITE_TIME,LastWriteTime),FailCount);
		}
		
		

		return FailCount==0;
	}
	bool ParsePacket(const CSmartStruct& Packet)
	{
		
		void * Pos=Packet.GetFirstMemberPosition();
		while(Pos)
		{
			WORD MemberID;
			CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
			switch(MemberID)
			{
			case SST_FILE_INFO_NAME:
				{
					Value.GetString(Name);
				}
				break;
			case SST_FILE_INFO_ATTRIBUTE:
				{
					Attribute=Value;
				}
				break;
			case SST_FILE_INFO_SIZE:
				{
					Size=Value;
				}
				break;
			case SST_FILE_INFO_CREATE_TIME:
				{
					CreateTime=Value;
				}
				break;
			case SST_FILE_INFO_LAST_ACCESS_TIME:
				{
					LastAccessTime=Value;
				}
				break;
			case SST_FILE_INFO_LAST_WRITE_TIME:
				{
					LastWriteTime=Value;
				}
				break;
			
			}
		}
		
		return true;
	}
	UINT GetSmartStructSize() const
	{
		UINT Size=0;
		{
			Size+=CSmartStruct::GetStringMemberSize(Name);
		}
		
		{
			Size+=CSmartStruct::GetFixMemberSize(sizeof(UINT));
		}
		
		{
			Size+=CSmartStruct::GetFixMemberSize(sizeof(UINT64));
		}
		
		{
			Size+=CSmartStruct::GetFixMemberSize(sizeof(UINT64));
		}
		
		{
			Size+=CSmartStruct::GetFixMemberSize(sizeof(UINT64));
		}
		
		{
			Size+=CSmartStruct::GetFixMemberSize(sizeof(UINT64));
		}
		
		
		return Size;
	}
	void CloneFrom(const FILE_INFO& Value)
	{
		{
			Name=Value.Name;
		}
		{
			Attribute=Value.Attribute;
		}
		{
			Size=Value.Size;
		}
		{
			CreateTime=Value.CreateTime;
		}
		{
			LastAccessTime=Value.LastAccessTime;
		}
		{
			LastWriteTime=Value.LastWriteTime;
		}
		
	}
	FILE_INFO& operator=(const FILE_INFO& Value)
	{
		CloneFrom(Value);
		return *this;
	}
	
	
	
	
	
	

};



//
struct FILE_INFO_LIST
{
	CEasyArray<FILE_INFO>		List;
	
	
	enum FILE_INFO_LIST_MEMBER_IDS
	{
		SST_FILE_INFO_LIST_LIST = 1,
		
	};
	
	
	FILE_INFO_LIST()
	{
		Clear();
	}
	FILE_INFO_LIST(const FILE_INFO_LIST& Value)
	{
		*this=Value;
	}
	void Clear()
	{
		List.SetTag(_T("StructData"));
		List.Clear();
		List.Create(16,8);
		
	}
	bool MakePacket(CSmartStruct& Packet) const
	{
		UINT FailCount=0;

		{
			CSmartStruct& ParentPacket=Packet;
			{
				CSmartArray Packet=ParentPacket.PrepareSubArray();
				for(size_t i=0;i<List.GetCount();i++)
				{
					CSmartStruct SubPacket=Packet.PrepareSubStruct();
					if(!List[i].MakePacket(SubPacket))
						FailCount++;
					if(!Packet.FinishMember(SubPacket.GetDataLen()))
						FailCount++;
				}
				if(!ParentPacket.FinishMember(SST_FILE_INFO_LIST_LIST,Packet.GetDataLen()))
					FailCount++;
			}
		}
		

		return FailCount==0;
	}
	bool ParsePacket(const CSmartStruct& Packet)
	{
		
		void * Pos=Packet.GetFirstMemberPosition();
		while(Pos)
		{
			WORD MemberID;
			CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
			switch(MemberID)
			{
			case SST_FILE_INFO_LIST_LIST:
				{
					List.Clear();
					CSmartArray Packet=Value;
					for(CSmartValue Value : Packet)
					{
						FILE_INFO& ArrayElement=*List.AddEmpty();
						ArrayElement.ParsePacket(Value);
						
					}
				}
				break;
			
			
			}
		}
		
		return true;
	}
	UINT GetSmartStructSize() const
	{
		UINT Size=0;
		{
			for(size_t i=0;i<List.GetCount();i++)
			{
				Size+=CSmartArray::GetStructMemberSize(List[i].GetSmartStructSize());
			}
			Size+=CSmartStruct::GetArrayMemberSize(0);
		}
		
		return Size;
	}
	void CloneFrom(const FILE_INFO_LIST& Value)
	{
		{
			List=Value.List;
		}
		
	}
	FILE_INFO_LIST& operator=(const FILE_INFO_LIST& Value)
	{
		CloneFrom(Value);
		return *this;
	}
	
	
	
	
	
	

};




