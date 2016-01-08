#pragma once

#include <iostream>
#include <vector>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <tbb/spin_rw_mutex.h>

typedef struct MemberInfo
{
	MemberInfo()
	{
		this->user_identifier	= 0;
		this->cell_phone		= 0;
		memset(this->team_name, 0, sizeof(this->team_name));
		this->user_flag			= 0;
		this->update			= 0;
		this->upt_time			= 0;
	}

	MemberInfo(int64_t user_identifier, int64_t cell_phone, char team_name[], int32_t user_flag, unsigned char update, int64_t upt_time)
	{
		this->user_identifier = user_identifier;
		this->cell_phone = cell_phone;
		strcpy(this->team_name, team_name);
		//memcpy(this->team_name, team_name, strlen(team_name));
		this->user_flag = user_flag;
		this->update = update;
		this->upt_time = upt_time;
	}

	int64_t			user_identifier;
	int64_t			cell_phone;
	char			team_name[100];
	int32_t			user_flag;
	unsigned char	update;
	int64_t			upt_time;
} MemberInfo;

struct MemberInfoUserIdentifierKey {};
struct MemberInfoCellPhoneKey {};

typedef boost::multi_index_container<
			MemberInfo,
			boost::multi_index::indexed_by<
				boost::multi_index::ordered_unique<
					boost::multi_index::tag<MemberInfoUserIdentifierKey>,
					BOOST_MULTI_INDEX_MEMBER(MemberInfo, int64_t, user_identifier)
				>,
				boost::multi_index::ordered_non_unique<
					boost::multi_index::tag<MemberInfoCellPhoneKey>,
					BOOST_MULTI_INDEX_MEMBER(MemberInfo, int64_t, cell_phone)
				>
			>
		> MemberInfoContainer;

class MemberInfoManager
{
public:
	typedef MemberInfoContainer::iterator UserNoIterator;
	typedef MemberInfoContainer::nth_index<1>::type::iterator CellPhoneIterator;
	typedef tbb::spin_rw_mutex Mutex;
	typedef Mutex::scoped_lock Lock;

	MemberInfoManager(void);
	~MemberInfoManager(void);

	// Write Lock
	// 새로운 데이터 입력
	bool Insert(int64_t user_identifier, int64_t cell_phone, char team_name[], int32_t user_flag, unsigned char update, int64_t upt_time);
	
	// 전화번호 목록을 이용해 현재 게임진행중인 유저들의 전화번호, 구단명 목록을 뽑는다.
	//int GetActiveUserList(std::vector<int64_t> cell_phone_list/*, MemberInfoRef& out_data*/);
	unsigned int GetActiveUserList(std::vector<int64_t>& cell_phone_list, unsigned char* pack_data, unsigned int pack_data_length);
	
	// 특정 유저ID의 업데이트 여부를 수정한다.
	bool ModifyForUpdatedStatus(int64_t user_identifier, unsigned char status);
	
	// 특정 유저ID의 upt_time을 수정한다.
	bool ModifyForUptTime(int64_t user_identifier, int64_t upt_time);

	// 유저ID로 데이터가 업데이트되었는지 여부 확인
	bool IsNeedToUpdateByUserID(int64_t user_identifier);

	// 휴대폰번호로 데이터가 업데이트되었는지 여부 확인
	bool IsNeedToUpdateByCellPhone(int64_t user_identifier);


	// 전화번호로 검색한다.
	bool FindByCellPhone(int64_t cell_phone, CellPhoneIterator& member_info);
	
	// 유저ID로 검색한다.
	bool FindByUserID(int64_t user_identifier, UserNoIterator& member_info);

	bool CheckUpdateStatusByUserID(int64_t user_identifier);

	bool GetTeamNameByUserID(int64_t user_identifier, std::string& team_name);

	// 유저ID로 검색하여 정보를 가져온다.
	bool GetDataByUserID(int64_t user_identifier, int64_t& cell_phone, std::string& team_name, int32_t& user_flag, unsigned char& update);

	// 구단명으로 검색하여 정보를 가져온다.
	bool GetDataByTeamName(std::string team_name, int64_t& user_identifier, int64_t& cell_phone, int32_t& user_flag, unsigned char& update);

	// 데이터를 적재한다.
	bool Load(MemberInfoContainer* memberinfo_container_);

	// 데이터를 다시 적재한다.
	bool Reload(void);

	// 초기화
	bool Initialize(void);

	// 데이터의 갯수를 반환한다.
	int64_t Size(void);
private:
	Mutex rw_mutex_;

	MemberInfoContainer* memberinfo_container_ptr_;
	std::vector<MemberInfoContainer> memberinfo_container_vec_;

	int memberinfo_container_flag_;
};

