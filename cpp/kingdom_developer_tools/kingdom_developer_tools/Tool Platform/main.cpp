#include "toolplatform.h"
#include "qt_windows.h"
#include "../kdt_translate_interface/kdt_translate_head.h"
#pragma comment(lib,"../debug/kdt_translate_interface.lib")
#include <QDebug>
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	qDebug() << "1";
	trans_param obj_param;
	strcpy(obj_param.m_szAppId,"20180309000133299");                    //appid
	strcpy(obj_param.m_szSecret_key,"VDZlUclYn255xwCSi7X2");            //��Կ
	strcpy(obj_param.m_szLanguageFrom,"zh");							//��Դ����
	strcpy(obj_param.m_szLanguageTo,"en");								//Ŀ������
	obj_param.m_vcTarget.push_back(std::string("ƻ��"));
	obj_param.m_vcTarget.push_back(std::string("������"));
	obj_param.m_vcTarget.push_back(std::string("����"));
	obj_param.m_vcTarget.push_back(std::string("ָ����Ч"));
	obj_param.m_vcTarget.push_back(std::string("��̶��ѧ"));

	kdt_trans_func(&obj_param);
	if (obj_param.m_nErrorCode == 0)
	{
		for (std::vector<std::string>::iterator iter = obj_param.m_vcResult.begin(); iter != obj_param.m_vcResult.end(); iter++)
		{
			qDebug() <<QString("������:")<< iter->c_str();
		}
	}
	ToolPlatform w;
	w.show();
	return a.exec();
}
