#ifndef _KDT_TRANSLATE_HEAD_
#define _KDT_TRANSLATE_HEAD_

#define ATOM_BUSI_EXPORTS

#include "compile_def.h"
#include "type_def.h"
#include "public_func.h"
#include <vector>

/* ֧�������б�
auto �Զ���� ; zh	���� ; en	Ӣ�� ; yue	���� ; wyw	������ ; jp	���� ; kor	���� ;
fra	���� ; spa	�������� ; th	̩�� ; ara	�������� ; ru	���� ; pt	�������� ;
de	���� ; it	������� ; el	ϣ���� ; nl	������ ; pl	������ ; bul	���������� ;
est	��ɳ������ ; dan	������ ; fin	������ ; cs	�ݿ��� ; rom	���������� ;
slo	˹���������� ; swe	����� ; hu	�������� ; cht	�������� ; vie	Խ���� ;
*/
typedef struct
{
	dt_vchar128			m_szAppId;                     //appid
	dt_vchar128			m_szSecret_key;                //��Կ
	dt_vchar2			m_szLanguageFrom;              //��Դ����
	dt_vchar2			m_szLanguageTo;                //Ŀ������
	std::vector<std::string> m_vcTarget;               //���������
	std::vector<std::string> m_vcResult;               //���
} trans_param;

extern "C"
{
	ATOM_BUSI_FUNC void kdt_trans_func(trans_param *ptr_param);
	

};

#endif