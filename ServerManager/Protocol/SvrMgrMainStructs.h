#pragma once



//
struct PROCESS_INFO
{
	UINT			ProcessID;
	CEasyString		ImageFile;
	UINT64			ImageFileTime;
	UINT64			CPUUsedTime;
	float			CPUUsed;
	UINT64			MemoryUsed;
	UINT64			VirtualMemoryUsed;
	
	
	enum PROCESS__INFO_MEMBER_IDS
	{
		SST_SERVICE_INFO_PROCESS_ID=1,
		SST_SERVICE_INFO_IMAGE_FILE=3,
		SST_SERVICE_INFO_IMAGE_FILE_TIME=4,
		SST_SERVICE_INFO_PUUSED_TIME=9,
		SST_SERVICE_INFO_PUUSED=6,
		SST_SERVICE_INFO_MEMORY_USED=7,
		SST_SERVICE_INFO_VIRTUAL_MEMORY_USED=8,
	
	};
	PROCESS_INFO()
	{
		Clear();
	}
	PROCESS_INFO(const PROCESS_INFO& Value)
	{
		*this=Value;
	}
	void Clear()
	{

		ProcessID=0;
		ImageFile.Clear();
		ImageFileTime=0;
		CPUUsedTime=0;
		CPUUsed=0;
		MemoryUsed=0;
		VirtualMemoryUsed=0;
		
	}
	bool MakePacket(CSmartStruct& Packet) const
	{
		UINT FailCount=0;


		{
			CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_SERVICE_INFO_PROCESS_ID,ProcessID),FailCount);
		}
		
		{
			CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_SERVICE_INFO_IMAGE_FILE,ImageFile),FailCount);
		}
		
		{
			CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_SERVICE_INFO_IMAGE_FILE_TIME,ImageFileTime),FailCount);
		}
		
		{
			CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_SERVICE_INFO_PUUSED_TIME,CPUUsedTime),FailCount);
		}
		
		{
			CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_SERVICE_INFO_PUUSED,CPUUsed),FailCount);
		}
		
		{
			CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_SERVICE_INFO_MEMORY_USED,MemoryUsed),FailCount);
		}
		
		{
			CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_SERVICE_INFO_VIRTUAL_MEMORY_USED,VirtualMemoryUsed),FailCount);
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
			case SST_SERVICE_INFO_PROCESS_ID:
				{
					ProcessID=Value;
					
				}
				break;
			case SST_SERVICE_INFO_IMAGE_FILE:
				{
					Value.GetString(ImageFile);
					
				}
				break;
			case SST_SERVICE_INFO_IMAGE_FILE_TIME:
				{
					ImageFileTime=Value;
					
				}
				break;
			case SST_SERVICE_INFO_PUUSED_TIME:
				{
					CPUUsedTime=Value;
					
				}
				break;
			case SST_SERVICE_INFO_PUUSED:
				{
					CPUUsed=Value;
					
				}
				break;
			case SST_SERVICE_INFO_MEMORY_USED:
				{
					MemoryUsed=Value;
					
				}
				break;
			case SST_SERVICE_INFO_VIRTUAL_MEMORY_USED:
				{
					VirtualMemoryUsed=Value;
					
				}
				break;
			
			}
		}
		
		return true;
	}
	UINT GetSmartStructSize() const
	{
		UINT Size=0;
		Size+=CSmartStruct::GetFixMemberSize(sizeof(UINT));
		Size+=CSmartStruct::GetStringMemberSize((UINT)ImageFile.GetLength());
		Size+=CSmartStruct::GetFixMemberSize(sizeof(UINT64));
		Size+=CSmartStruct::GetFixMemberSize(sizeof(UINT64));
		Size+=CSmartStruct::GetFixMemberSize(sizeof(float));
		Size+=CSmartStruct::GetFixMemberSize(sizeof(UINT64));
		Size+=CSmartStruct::GetFixMemberSize(sizeof(UINT64));
		
		return Size;
	}
	void CloneFrom(const PROCESS_INFO& Value)
	{
		{
			ProcessID=Value.ProcessID;
		}
		{
			ImageFile=Value.ImageFile;
		}
		{
			ImageFileTime=Value.ImageFileTime;
		}
		{
			CPUUsedTime=Value.CPUUsedTime;
		}
		{
			CPUUsed=Value.CPUUsed;
		}
		{
			MemoryUsed=Value.MemoryUsed;
		}
		{
			VirtualMemoryUsed=Value.VirtualMemoryUsed;
		}
		
	}
	PROCESS_INFO& operator=(const PROCESS_INFO& Value)
	{
		CloneFrom(Value);
		return *this;
	}
	 
};



//
struct PROCESS_INFO_LIST
{
	CEasyArray<PROCESS_INFO>	List;
	
	
	enum PROCESS__INFO__LIST_MEMBER_IDS
	{
		SST_SERVICE_INFO_LIST_LIST=1,
	
	};
	PROCESS_INFO_LIST()
	{
		Clear();
	}
	PROCESS_INFO_LIST(const PROCESS_INFO_LIST& Value)
	{
		*this=Value;
	}
	void Clear()
	{

		List.Clear();
		List.Create(16,8);
		
	}
	bool MakePacket(CSmartStruct& Packet) const
	{
		UINT FailCount=0;


		{
			UINT BufferSize;
			void * pBuffer=Packet.PrepareMember(BufferSize);
			{
				CSmartStruct Packet(pBuffer,BufferSize,true);
				for(size_t i=0;i<List.GetCount();i++)
				{
					UINT BufferSize;
					void * pBuffer=Packet.PrepareMember(BufferSize);
					CSmartStruct SubPacket(pBuffer,BufferSize,true);
					if(!List[i].MakePacket(SubPacket)) FailCount++;
					Packet.FinishMember(SST_SERVICE_INFO_LIST_LIST,SubPacket.GetDataLen());
				}
				BufferSize=Packet.GetDataLen();
			}
			Packet.FinishMember(SST_SERVICE_INFO_LIST_LIST,BufferSize);
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
			case SST_SERVICE_INFO_LIST_LIST:
				{
					List.Clear();
					CSmartStruct Packet=Value;
					void * Pos=Packet.GetFirstMemberPosition();
					while(Pos)
					{
						WORD MemberID;
						CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
						switch(MemberID)
						{
						case SST_SERVICE_INFO_LIST_LIST:
							{	
								PROCESS_INFO	ArrayElement;
								ArrayElement.ParsePacket(Value);
								
								List.Add(ArrayElement);
							}
							break;
						}
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
		for(size_t i=0;i<List.GetCount();i++)
		{
			Size+=CSmartStruct::GetStructMemberSize(List[i].GetSmartStructSize())
		;
		}
		Size+=CSmartStruct::GetStructMemberSize(0);
		
		return Size;
	}
	void CloneFrom(const PROCESS_INFO_LIST& Value)
	{
		{
			List=Value.List;
		}
		
	}
	PROCESS_INFO_LIST& operator=(const PROCESS_INFO_LIST& Value)
	{
		CloneFrom(Value);
		return *this;
	}
	 
};



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
	
	
	enum NET__ADAPTER__INFO_MEMBER_IDS
	{
		SST_NET_ADAPTER_INFO_INDEX=1,
		SST_NET_ADAPTER_INFO_IPADDRESS_LIST=2,
		SST_NET_ADAPTER_INFO_NAME=7,
		SST_NET_ADAPTER_INFO_SEND_BYTES=5,
		SST_NET_ADAPTER_INFO_SEND_FLUX=3,
		SST_NET_ADAPTER_INFO_RECV_BYTES=6,
		SST_NET_ADAPTER_INFO_RECV_FLUX=4,
	
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
			UINT BufferSize;
			void * pBuffer=Packet.PrepareMember(BufferSize);
			{
				CSmartStruct Packet(pBuffer,BufferSize,true);
				for(size_t i=0;i<IPAddressList.GetCount();i++)
				{
					CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_NET_ADAPTER_INFO_IPADDRESS_LIST,IPAddressList[i]),FailCount);
				}
				BufferSize=Packet.GetDataLen();
			}
			Packet.FinishMember(SST_NET_ADAPTER_INFO_IPADDRESS_LIST,BufferSize);
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
					CSmartStruct Packet=Value;
					void * Pos=Packet.GetFirstMemberPosition();
					while(Pos)
					{
						WORD MemberID;
						CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
						switch(MemberID)
						{
						case SST_NET_ADAPTER_INFO_IPADDRESS_LIST:
							{	
								CEasyString	ArrayElement;
								Value.GetString(ArrayElement);
								
								IPAddressList.Add(ArrayElement);
							}
							break;
						}
					}
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
		Size+=CSmartStruct::GetFixMemberSize(sizeof(UINT));
		for(size_t i=0;i<IPAddressList.GetCount();i++)
		{
			Size+=CSmartStruct::GetStringMemberSize((UINT)IPAddressList[i].GetLength());
		}
		Size+=CSmartStruct::GetStructMemberSize(0);
		Size+=CSmartStruct::GetStringMemberSize((UINT)Name.GetLength());
		Size+=CSmartStruct::GetFixMemberSize(sizeof(UINT64));
		Size+=CSmartStruct::GetFixMemberSize(sizeof(float));
		Size+=CSmartStruct::GetFixMemberSize(sizeof(UINT64));
		Size+=CSmartStruct::GetFixMemberSize(sizeof(float));
		
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
	
	
	enum NET__ADAPTER__INFO__LIST_MEMBER_IDS
	{
		SST_NET_ADAPTER_INFO_LIST_LIST=1,
	
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

		List.Clear();
		List.Create(16,8);
		
	}
	bool MakePacket(CSmartStruct& Packet) const
	{
		UINT FailCount=0;


		{
			UINT BufferSize;
			void * pBuffer=Packet.PrepareMember(BufferSize);
			{
				CSmartStruct Packet(pBuffer,BufferSize,true);
				for(size_t i=0;i<List.GetCount();i++)
				{
					UINT BufferSize;
					void * pBuffer=Packet.PrepareMember(BufferSize);
					CSmartStruct SubPacket(pBuffer,BufferSize,true);
					if(!List[i].MakePacket(SubPacket)) FailCount++;
					Packet.FinishMember(SST_NET_ADAPTER_INFO_LIST_LIST,SubPacket.GetDataLen());
				}
				BufferSize=Packet.GetDataLen();
			}
			Packet.FinishMember(SST_NET_ADAPTER_INFO_LIST_LIST,BufferSize);
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
					CSmartStruct Packet=Value;
					void * Pos=Packet.GetFirstMemberPosition();
					while(Pos)
					{
						WORD MemberID;
						CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
						switch(MemberID)
						{
						case SST_NET_ADAPTER_INFO_LIST_LIST:
							{	
								NET_ADAPTER_INFO	ArrayElement;
								ArrayElement.ParsePacket(Value);
								
								List.Add(ArrayElement);
							}
							break;
						}
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
		for(size_t i=0;i<List.GetCount();i++)
		{
			Size+=CSmartStruct::GetStructMemberSize(List[i].GetSmartStructSize())
		;
		}
		Size+=CSmartStruct::GetStructMemberSize(0);
		
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
	
	
	enum FILE__INFO_MEMBER_IDS
	{
		SST_FILE_INFO_NAME=1,
		SST_FILE_INFO_ATTRIBUTE=2,
		SST_FILE_INFO_SIZE=3,
		SST_FILE_INFO_CREATE_TIME=4,
		SST_FILE_INFO_LAST_ACCESS_TIME=5,
		SST_FILE_INFO_LAST_WRITE_TIME=6,
	
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
		Size+=CSmartStruct::GetStringMemberSize((UINT)Name.GetLength());
		Size+=CSmartStruct::GetFixMemberSize(sizeof(UINT));
		Size+=CSmartStruct::GetFixMemberSize(sizeof(UINT64));
		Size+=CSmartStruct::GetFixMemberSize(sizeof(UINT64));
		Size+=CSmartStruct::GetFixMemberSize(sizeof(UINT64));
		Size+=CSmartStruct::GetFixMemberSize(sizeof(UINT64));
		
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
	
	
	enum FILE__INFO__LIST_MEMBER_IDS
	{
		SST_FILE_INFO_LIST_LIST=1,
	
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

		List.Clear();
		List.Create(16,8);
		
	}
	bool MakePacket(CSmartStruct& Packet) const
	{
		UINT FailCount=0;


		{
			UINT BufferSize;
			void * pBuffer=Packet.PrepareMember(BufferSize);
			{
				CSmartStruct Packet(pBuffer,BufferSize,true);
				for(size_t i=0;i<List.GetCount();i++)
				{
					UINT BufferSize;
					void * pBuffer=Packet.PrepareMember(BufferSize);
					CSmartStruct SubPacket(pBuffer,BufferSize,true);
					if(!List[i].MakePacket(SubPacket)) FailCount++;
					Packet.FinishMember(SST_FILE_INFO_LIST_LIST,SubPacket.GetDataLen());
				}
				BufferSize=Packet.GetDataLen();
			}
			Packet.FinishMember(SST_FILE_INFO_LIST_LIST,BufferSize);
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
					CSmartStruct Packet=Value;
					void * Pos=Packet.GetFirstMemberPosition();
					while(Pos)
					{
						WORD MemberID;
						CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
						switch(MemberID)
						{
						case SST_FILE_INFO_LIST_LIST:
							{	
								FILE_INFO	ArrayElement;
								ArrayElement.ParsePacket(Value);
								
								List.Add(ArrayElement);
							}
							break;
						}
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
		for(size_t i=0;i<List.GetCount();i++)
		{
			Size+=CSmartStruct::GetStructMemberSize(List[i].GetSmartStructSize())
		;
		}
		Size+=CSmartStruct::GetStructMemberSize(0);
		
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



//
struct SERVICE_INFO
{
	UINT			ServiceID;
	CEasyString		Name;
	BYTE			Type;
	CEasyString		ImageFilePath;
	UINT64			ImageFileTime;
	CEasyString		WorkDir;
	CEasyString		StartupParam;
	UINT			ProcessID;
	UINT64			RecentTime;
	BYTE			Status;
	float			CPUUsed;
	UINT64			MemoryUsed;
	UINT64			VirtualMemoryUsed;
	BYTE			LastOperation;
	UINT			LastStatusChangeTime;
	UINT			RestartupTime;
	
	
	enum SERVICE__INFO_MEMBER_IDS
	{
		SST_SERVICE_INFO_SERVICE_ID=1,
		SST_SERVICE_INFO_NAME=13,
		SST_SERVICE_INFO_TYPE=2,
		SST_SERVICE_INFO_IMAGE_FILE_PATH=3,
		SST_SERVICE_INFO_IMAGE_FILE_TIME=12,
		SST_SERVICE_INFO_WORK_DIR=4,
		SST_SERVICE_INFO_STARTUP_PARAM=5,
		SST_SERVICE_INFO_PROCESS_ID=6,
		SST_SERVICE_INFO_RECENT_TIME=7,
		SST_SERVICE_INFO_STATUS=8,
		SST_SERVICE_INFO_PUUSED=9,
		SST_SERVICE_INFO_MEMORY_USED=10,
		SST_SERVICE_INFO_VIRTUAL_MEMORY_USED=11,
		SST_SERVICE_INFO_LAST_OPERATION=14,
		SST_SERVICE_INFO_LAST_STATUS_CHANGE_TIME=15,
		SST_SERVICE_INFO_RESTARTUP_TIME=16,
	
	};
	SERVICE_INFO()
	{
		Clear();
	}
	SERVICE_INFO(const SERVICE_INFO& Value)
	{
		*this=Value;
	}
	void Clear()
	{

		ServiceID=0;
		Name.Clear();
		Type=0;
		ImageFilePath.Clear();
		ImageFileTime=0;
		WorkDir.Clear();
		StartupParam.Clear();
		ProcessID=0;
		RecentTime=0;
		Status=0;
		CPUUsed=0;
		MemoryUsed=0;
		VirtualMemoryUsed=0;
		LastOperation=0;
		LastStatusChangeTime=0;
		RestartupTime=0;
		
	}
	bool MakePacket(CSmartStruct& Packet) const
	{
		UINT FailCount=0;


		{
			CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_SERVICE_INFO_SERVICE_ID,ServiceID),FailCount);
		}
		
		{
			CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_SERVICE_INFO_NAME,Name),FailCount);
		}
		
		{
			CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_SERVICE_INFO_TYPE,Type),FailCount);
		}
		
		{
			CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_SERVICE_INFO_IMAGE_FILE_PATH,ImageFilePath),FailCount);
		}
		
		{
			CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_SERVICE_INFO_IMAGE_FILE_TIME,ImageFileTime),FailCount);
		}
		
		{
			CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_SERVICE_INFO_WORK_DIR,WorkDir),FailCount);
		}
		
		{
			CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_SERVICE_INFO_STARTUP_PARAM,StartupParam),FailCount);
		}
		
		{
			CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_SERVICE_INFO_PROCESS_ID,ProcessID),FailCount);
		}
		
		{
			CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_SERVICE_INFO_RECENT_TIME,RecentTime),FailCount);
		}
		
		{
			CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_SERVICE_INFO_STATUS,Status),FailCount);
		}
		
		{
			CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_SERVICE_INFO_PUUSED,CPUUsed),FailCount);
		}
		
		{
			CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_SERVICE_INFO_MEMORY_USED,MemoryUsed),FailCount);
		}
		
		{
			CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_SERVICE_INFO_VIRTUAL_MEMORY_USED,VirtualMemoryUsed),FailCount);
		}
		
		{
			CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_SERVICE_INFO_LAST_OPERATION,LastOperation),FailCount);
		}
		
		{
			CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_SERVICE_INFO_LAST_STATUS_CHANGE_TIME,LastStatusChangeTime),FailCount);
		}
		
		{
			CHECK_SMART_STRUCT_ADD(Packet.AddMember(SST_SERVICE_INFO_RESTARTUP_TIME,RestartupTime),FailCount);
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
			case SST_SERVICE_INFO_SERVICE_ID:
				{
					ServiceID=Value;
					
				}
				break;
			case SST_SERVICE_INFO_NAME:
				{
					Value.GetString(Name);
					
				}
				break;
			case SST_SERVICE_INFO_TYPE:
				{
					Type=Value;
					
				}
				break;
			case SST_SERVICE_INFO_IMAGE_FILE_PATH:
				{
					Value.GetString(ImageFilePath);
					
				}
				break;
			case SST_SERVICE_INFO_IMAGE_FILE_TIME:
				{
					ImageFileTime=Value;
					
				}
				break;
			case SST_SERVICE_INFO_WORK_DIR:
				{
					Value.GetString(WorkDir);
					
				}
				break;
			case SST_SERVICE_INFO_STARTUP_PARAM:
				{
					Value.GetString(StartupParam);
					
				}
				break;
			case SST_SERVICE_INFO_PROCESS_ID:
				{
					ProcessID=Value;
					
				}
				break;
			case SST_SERVICE_INFO_RECENT_TIME:
				{
					RecentTime=Value;
					
				}
				break;
			case SST_SERVICE_INFO_STATUS:
				{
					Status=Value;
					
				}
				break;
			case SST_SERVICE_INFO_PUUSED:
				{
					CPUUsed=Value;
					
				}
				break;
			case SST_SERVICE_INFO_MEMORY_USED:
				{
					MemoryUsed=Value;
					
				}
				break;
			case SST_SERVICE_INFO_VIRTUAL_MEMORY_USED:
				{
					VirtualMemoryUsed=Value;
					
				}
				break;
			case SST_SERVICE_INFO_LAST_OPERATION:
				{
					LastOperation=Value;
					
				}
				break;
			case SST_SERVICE_INFO_LAST_STATUS_CHANGE_TIME:
				{
					LastStatusChangeTime=Value;
					
				}
				break;
			case SST_SERVICE_INFO_RESTARTUP_TIME:
				{
					RestartupTime=Value;
					
				}
				break;
			
			}
		}
		
		return true;
	}
	UINT GetSmartStructSize() const
	{
		UINT Size=0;
		Size+=CSmartStruct::GetFixMemberSize(sizeof(UINT));
		Size+=CSmartStruct::GetStringMemberSize((UINT)Name.GetLength());
		Size+=CSmartStruct::GetFixMemberSize(sizeof(BYTE));
		Size+=CSmartStruct::GetStringMemberSize((UINT)ImageFilePath.GetLength());
		Size+=CSmartStruct::GetFixMemberSize(sizeof(UINT64));
		Size+=CSmartStruct::GetStringMemberSize((UINT)WorkDir.GetLength());
		Size+=CSmartStruct::GetStringMemberSize((UINT)StartupParam.GetLength());
		Size+=CSmartStruct::GetFixMemberSize(sizeof(UINT));
		Size+=CSmartStruct::GetFixMemberSize(sizeof(UINT64));
		Size+=CSmartStruct::GetFixMemberSize(sizeof(BYTE));
		Size+=CSmartStruct::GetFixMemberSize(sizeof(float));
		Size+=CSmartStruct::GetFixMemberSize(sizeof(UINT64));
		Size+=CSmartStruct::GetFixMemberSize(sizeof(UINT64));
		Size+=CSmartStruct::GetFixMemberSize(sizeof(BYTE));
		Size+=CSmartStruct::GetFixMemberSize(sizeof(UINT));
		Size+=CSmartStruct::GetFixMemberSize(sizeof(UINT));
		
		return Size;
	}
	void CloneFrom(const SERVICE_INFO& Value)
	{
		{
			ServiceID=Value.ServiceID;
		}
		{
			Name=Value.Name;
		}
		{
			Type=Value.Type;
		}
		{
			ImageFilePath=Value.ImageFilePath;
		}
		{
			ImageFileTime=Value.ImageFileTime;
		}
		{
			WorkDir=Value.WorkDir;
		}
		{
			StartupParam=Value.StartupParam;
		}
		{
			ProcessID=Value.ProcessID;
		}
		{
			RecentTime=Value.RecentTime;
		}
		{
			Status=Value.Status;
		}
		{
			CPUUsed=Value.CPUUsed;
		}
		{
			MemoryUsed=Value.MemoryUsed;
		}
		{
			VirtualMemoryUsed=Value.VirtualMemoryUsed;
		}
		{
			LastOperation=Value.LastOperation;
		}
		{
			LastStatusChangeTime=Value.LastStatusChangeTime;
		}
		{
			RestartupTime=Value.RestartupTime;
		}
		
	}
	SERVICE_INFO& operator=(const SERVICE_INFO& Value)
	{
		CloneFrom(Value);
		return *this;
	}
	 
};



//
struct SERVICE_INFO_LIST
{
	CEasyArray<SERVICE_INFO>	List;
	
	
	enum SERVICE__INFO__LIST_MEMBER_IDS
	{
		SST_SERVICE_INFO_LIST_LIST=1,
	
	};
	SERVICE_INFO_LIST()
	{
		Clear();
	}
	SERVICE_INFO_LIST(const SERVICE_INFO_LIST& Value)
	{
		*this=Value;
	}
	void Clear()
	{

		List.Clear();
		List.Create(16,8);
		
	}
	bool MakePacket(CSmartStruct& Packet) const
	{
		UINT FailCount=0;


		{
			UINT BufferSize;
			void * pBuffer=Packet.PrepareMember(BufferSize);
			{
				CSmartStruct Packet(pBuffer,BufferSize,true);
				for(size_t i=0;i<List.GetCount();i++)
				{
					UINT BufferSize;
					void * pBuffer=Packet.PrepareMember(BufferSize);
					CSmartStruct SubPacket(pBuffer,BufferSize,true);
					if(!List[i].MakePacket(SubPacket)) FailCount++;
					Packet.FinishMember(SST_SERVICE_INFO_LIST_LIST,SubPacket.GetDataLen());
				}
				BufferSize=Packet.GetDataLen();
			}
			Packet.FinishMember(SST_SERVICE_INFO_LIST_LIST,BufferSize);
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
			case SST_SERVICE_INFO_LIST_LIST:
				{
					List.Clear();
					CSmartStruct Packet=Value;
					void * Pos=Packet.GetFirstMemberPosition();
					while(Pos)
					{
						WORD MemberID;
						CSmartValue Value=Packet.GetNextMember(Pos,MemberID);
						switch(MemberID)
						{
						case SST_SERVICE_INFO_LIST_LIST:
							{	
								SERVICE_INFO	ArrayElement;
								ArrayElement.ParsePacket(Value);
								
								List.Add(ArrayElement);
							}
							break;
						}
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
		for(size_t i=0;i<List.GetCount();i++)
		{
			Size+=CSmartStruct::GetStructMemberSize(List[i].GetSmartStructSize())
		;
		}
		Size+=CSmartStruct::GetStructMemberSize(0);
		
		return Size;
	}
	void CloneFrom(const SERVICE_INFO_LIST& Value)
	{
		{
			List=Value.List;
		}
		
	}
	SERVICE_INFO_LIST& operator=(const SERVICE_INFO_LIST& Value)
	{
		CloneFrom(Value);
		return *this;
	}
	 
};




