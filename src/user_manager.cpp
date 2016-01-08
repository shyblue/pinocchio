#include "user_manager.h"

MemberInfoManager::MemberInfoManager(void)
	:	memberinfo_container_flag_(1)
{
	for (int i = 0; i < 2; i++)
	{
		MemberInfoContainer memberinfo_container_;
		memberinfo_container_vec_.push_back(memberinfo_container_);
	}

	memberinfo_container_ptr_ = &memberinfo_container_vec_[0];
}


MemberInfoManager::~MemberInfoManager(void)
{
}

// 초기화
bool MemberInfoManager::Initialize(void)
{
	bool result = Load(memberinfo_container_ptr_);

	return result;
}

// 데이터를 적재한다.
bool MemberInfoManager::Load(MemberInfoContainer* memberinfo_container_)
{
	DBHelper helper(0,0);

	{
		// MemberInfo에 데이터 삽입 시작
		std::string query = "EXEC [BASEBALL_MANAGER].[ServerInterface].[USP_MEMBER_INFO_GetAll_ForFriend_R]";
		helper->Prepared(query);

		int64_t user_no = 0;
		char user_device_ctn[100] = {0,};
		char team_name[100] = {0,};
		int32_t user_flag = 0;
		int64_t upt_time = 0;

		int member_count = 0;

		if (true == helper->Execute(st_util::database::SQLManager::EXECUTE_BIND))
		{
			helper->BindColBigInt(1, user_no);
			helper->BindColChar(2, user_device_ctn, (SQLINTEGER)sizeof(user_device_ctn));
			helper->BindColChar(3, team_name, (SQLINTEGER)sizeof(team_name));
			helper->BindColInt(4, user_flag);
			helper->BindColBigInt(5, upt_time);

			memberinfo_container_->clear();

			while (helper->Fetch())
			{
				// 빈값이 있을 수 있으므로 이렇게 처리
				int64_t temp_user_device_ctn = 0;
				if (strlen(user_device_ctn) > 0)
				{
#ifdef __linux
					temp_user_device_ctn = atoll(user_device_ctn);
#else
					temp_user_device_ctn = _atoi64(user_device_ctn);
#endif
					MemberInfo member_info(user_no, temp_user_device_ctn, team_name, user_flag, 1, upt_time);
					memberinfo_container_->insert(member_info);

					member_count++;
				}
			}

			GLOG_INFO("Add data to MemberInfo. Data count : %d", member_count);
		}
		else
		{
			GLOG_INFO("WARNING!!! Add data to MemberInfo FAILED!!!");

			return false;
		}
	}

	return true;
}


bool MemberInfoManager::Reload(void)
{
	MemberInfoContainer* target_container = NULL;

	if (memberinfo_container_flag_ == 1)							// 이 상태일 때 포인터는 0번째의 멀티인덱스를 바라본다.
	{
		target_container = &memberinfo_container_vec_[1];			// 데이터를 로드할 대상은 1번째 멀티인덱스로
	}
	else															// 이 상태일 때 포인터는 1번째의 멀티인덱스를 바라본다.
	{
		target_container = &memberinfo_container_vec_[0];			// 데이터를 로드할 대상은 0번째 멀티인덱스로
	}

	Load(target_container);											// 해당 컨테이너에 데이터를 로드한다.

	{
		Lock lock(rw_mutex_, false);								// 읽기락을 걸고
		memberinfo_container_ptr_ = target_container;				// 데이터를 모두 로드했다면 해당 컨테이너로 포인터를 바꿔준다.
	}

	memberinfo_container_flag_ = memberinfo_container_flag_ * -1;	// 플래그를 변경해준다.

	return true;
}

// 새로운 데이터 입력
bool MemberInfoManager::Insert(int64_t user_identifier, int64_t cell_phone, char team_name[], int32_t user_flag, unsigned char update, int64_t upt_time)
{
	Lock lock(rw_mutex_, true);

	MemberInfo member_info(user_identifier, cell_phone, team_name, user_flag, update, upt_time);

	std::pair<MemberInfoContainer::iterator, bool> p = memberinfo_container_ptr_->insert(member_info);

	return p.second;
}

// 전화번호로 검색한다.
bool MemberInfoManager::FindByCellPhone(int64_t cell_phone, CellPhoneIterator& member_info)
{
	Lock lock(rw_mutex_, false);

	// GLOG_DEBUG("function FindByCellPhone cell_phone(%lld)", cell_phone);

	typedef const MemberInfoContainer::nth_index<1>::type SecondIndexType;
	const MemberInfoContainer::nth_index<1>::type &cell_phone_index = memberinfo_container_ptr_->get<1>();

	// 검색용 이터레이터 (전화번호로 검색)
	std::pair<CellPhoneIterator, CellPhoneIterator> range_iter = cell_phone_index.equal_range(cell_phone);

	// GLOG_DEBUG("cell_phone_index.count(%d)", cell_phone_index.count(cell_phone));

	// 검색 결과가 없다면 검색오류로 처리하면 되므로 더이상 처리할 필요가 없다.
	if (cell_phone_index.count(cell_phone) <= 0)
	{
		return false;
	}

	// 검색 결과가 1개라면 해당하는 정보만 넘기면 된다.
	// 1개 이상이면 데이터들을 upt_time으로 비교하고 가장 최신 것을 리턴하도록 해야한다.
	// 유저가 기기변경을 하면 전화번호가 같은 데이터가 생겨나므로 이럴 때는 가장 마지막에 갱신된 데이터를 리턴해야한다.

	int64_t max_upt_time = 0;

	while (range_iter.first != range_iter.second)
	{
		// GLOG_DEBUG("max_upt_time(%lld)", max_upt_time);
		// GLOG_DEBUG("range_iter.first->upt_time(%lld)", range_iter.first->upt_time);

		if (range_iter.first->upt_time > max_upt_time)
		{
			max_upt_time = range_iter.first->upt_time;
			member_info = range_iter.first;

			// GLOG_DEBUG("member_info changed");
		}

		++range_iter.first;
	}

	GLOG_DEBUG("cell_phone : %lld", cell_phone);
	GLOG_DEBUG("upt_time : %lld", member_info->upt_time);

	return true;
}

bool MemberInfoManager::FindByUserID(int64_t user_identifier, UserNoIterator& member_info)
{
	member_info = memberinfo_container_ptr_->find(user_identifier);
	if (member_info != memberinfo_container_ptr_->end())
	{
		return true;
	}
	
	return false;
}

bool MemberInfoManager::GetTeamNameByUserID(int64_t user_identifier, std::string& team_name)
{
	Lock lock(rw_mutex_, false);

	UserNoIterator member_info = memberinfo_container_ptr_->find(user_identifier);
	if (member_info != memberinfo_container_ptr_->end())
	{
		team_name = std::string(member_info->team_name);
		return true;
	}

	return false;
}

bool MemberInfoManager::CheckUpdateStatusByUserID(int64_t user_identifier)
{
	Lock lock(rw_mutex_, false);

	UserNoIterator member_info = memberinfo_container_ptr_->find(user_identifier);
	return (member_info != memberinfo_container_ptr_->end() && (*member_info).update == 1);
}

unsigned int MemberInfoManager::GetActiveUserList(std::vector<int64_t>& cell_phone_list, unsigned char* pack_data, unsigned int pack_data_length)
{
	st_util::pack::InBuffer buffer(pack_data + sizeof(unsigned short), pack_data_length);
	
	unsigned short count = 0;

	std::vector<int64_t>::iterator itr;
	for (itr = cell_phone_list.begin(); itr != cell_phone_list.end(); itr++)
	{
		CellPhoneIterator cell_phone_iter;
		if (true == FindByCellPhone(*itr, cell_phone_iter))
		{
			// 패킷 구성 기능 구현
			buffer.SetInt64(cell_phone_iter->cell_phone);
			buffer.SetUShort(static_cast<unsigned short>(strlen(cell_phone_iter->team_name)));
			buffer.SetByte((unsigned char*)cell_phone_iter->team_name);

			++count;
		}
	}

	memcpy(pack_data, &count, sizeof(count));
	// memcpy(pack_data, buffer, buffer.buffer_length_());
	
	return static_cast<unsigned int>(buffer.get_position());
}

// 업데이트 필요 여부를 수정한다. (수정에 성공하면 true, 해당 휴대폰번호의 유저가 없으면 false)
bool MemberInfoManager::ModifyForUpdatedStatus(int64_t user_identifier, unsigned char status)
{
	Lock lock(rw_mutex_, true);

	UserNoIterator iter = memberinfo_container_ptr_->find(user_identifier);
	if (iter != memberinfo_container_ptr_->end())
	{
		iter.get_node()->value().update = status;
		return true;
	}
	
	return false;
}

// 특정 유저ID의 upt_time을 수정한다. (수정에 성공하면 true, 해당 휴대폰번호의 유저가 없으면 false)
bool MemberInfoManager::ModifyForUptTime(int64_t user_identifier, int64_t upt_time)
{
	Lock lock(rw_mutex_, true);

	UserNoIterator iter = memberinfo_container_ptr_->find(user_identifier);
	if (iter != memberinfo_container_ptr_->end())
	{
		iter.get_node()->value().upt_time = upt_time;
	}

	return false;
}

// 유저ID로 데이터가 업데이트되었는지 여부 확인
bool MemberInfoManager::IsNeedToUpdateByUserID(int64_t user_identifier)
{
	Lock lock(rw_mutex_, false);

	MemberInfoContainer::iterator memberinfo_itr = memberinfo_container_ptr_->find(user_identifier); // 내 전화번호로 MemberInfo에서 검색해서

	if (memberinfo_itr != memberinfo_container_ptr_->end())
	{
		unsigned char need_update = memberinfo_itr->update; // 업데이트 필요 여부를 구한 다음

		if (need_update != 0)	// 업데이트 필요하다면
		{
			return true;
		}
	}

	return false;	// MemberInfoContainer에서 찾을 수 없거나 업데이트가 필요 없다면
}

// 휴대폰번호로 데이터가 업데이트되었는지 여부 확인
bool MemberInfoManager::IsNeedToUpdateByCellPhone(int64_t cell_phone)
{
	Lock lock(rw_mutex_, false);

	const MemberInfoContainer::nth_index<1>::type &cell_phone_index = memberinfo_container_ptr_->get<1>();
	typedef const MemberInfoContainer::nth_index<1>::type SecondIndexType;
	SecondIndexType::iterator itr = cell_phone_index.find(cell_phone);

	if (0 > cell_phone_index.count(cell_phone))
	{
		if (itr->update == 0)
		{
			return true;
		}
	}

	return false;
}


bool MemberInfoManager::GetDataByUserID(int64_t user_identifier, int64_t& cell_phone, std::string& team_name, int32_t& user_flag, unsigned char& update)
{
	Lock lock(rw_mutex_, false);

	UserNoIterator member_info = memberinfo_container_ptr_->find(user_identifier);
	if (member_info != memberinfo_container_ptr_->end())
	{
		cell_phone	= member_info->cell_phone;
		team_name	= std::string(member_info->team_name);
		user_flag	= member_info->user_flag;
		update		= member_info->update;
		return true;
	}

	return false;
}


bool MemberInfoManager::GetDataByTeamName(std::string team_name, int64_t& user_identifier, int64_t& cell_phone, int32_t& user_flag, unsigned char& update)
{
	Lock lock(rw_mutex_, false);

	MemberInfoContainer::iterator itr;
	for (itr = memberinfo_container_ptr_->begin(); itr != memberinfo_container_ptr_->end(); itr++)
	{
		if (std::string(itr->team_name) == team_name)
		{
			user_identifier	= itr->user_identifier;
			cell_phone		= itr->cell_phone;
			user_flag		= itr->user_flag;
			update			= itr->update;

			return true;
		}
	}

	return false;
}
