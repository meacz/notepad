// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// w.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

#include "stdafx.h"

CString SaveIniFilePath() {
	CString str;
	SHGetFolderPath(NULL, CSIDL_PROFILE, NULL, 0, str.GetBuffer(MAX_PATH));
	str.ReleaseBuffer();
	return str;
}