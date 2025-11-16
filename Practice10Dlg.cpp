
// Practice10Dlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "Practice10.h"
#include "Practice10Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPractice10Dlg 대화 상자



CPractice10Dlg::CPractice10Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PRACTICE10_DIALOG, pParent)
	, m_strName(_T(""))
	, m_strContact(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPractice10Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MEMBERS, m_listMembers);
	DDX_Control(pDX, IDC_MFCPROPERTYGRID_MEMBER, m_propMember);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_CONTACT, m_strContact);
}

BEGIN_MESSAGE_MAP(CPractice10Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CPractice10Dlg::OnClickedButtonAdd)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_MEMBERS, &CPractice10Dlg::OnItemchangedListMembers)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CPractice10Dlg::OnClickedButtonDelete)
	ON_COMMAND(IDC_RADIO_MALE, &CPractice10Dlg::OnRadioMale)
	ON_COMMAND(IDC_RADIO_FEMALE, &CPractice10Dlg::OnRadioFemale)
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, &CPractice10Dlg::OnPropertyChanged)
	ON_STN_CLICKED(IDC_MFCPROPERTYGRID_MEMBER, &CPractice10Dlg::OnStnClickedMfcpropertygridMember)
END_MESSAGE_MAP()


// CPractice10Dlg 메시지 처리기

BOOL CPractice10Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	//20221871 인석진
	m_bSex = true;
	m_nSelectedMember = -1;

	//list control column 4개 추가
	m_listMembers.InsertColumn(0, _T("번호"), LVCFMT_LEFT, 50);
	m_listMembers.InsertColumn(1, _T("이름"), LVCFMT_LEFT, 100);
	m_listMembers.InsertColumn(2, _T("성별"), LVCFMT_LEFT, 60);
	m_listMembers.InsertColumn(3, _T("연락처"), LVCFMT_LEFT, 150);

	//List Control 스타일 설정
	m_listMembers.SetExtendedStyle(
		LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT
	);
	//Property Grid 생성 & 초기화 호출
	UpdatePropGrid(_T(""), true, _T(""));

	//라디오 버튼 초기화
	CheckDlgButton(IDC_RADIO_MALE, BST_CHECKED);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CPractice10Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CPractice10Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CPractice10Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CPractice10Dlg::UpdatePropGrid(CString name, bool sex, CString contact)
{
	// TODO: 여기에 구현 코드 추가.
	m_propMember.RemoveAll();
	m_propMember.EnableHeaderCtrl(FALSE);
	m_propMember.EnableDescriptionArea(FALSE);
	m_propMember.SetVSDotNetLook(TRUE);

	// 그룹 1: 회원 정보
	CMFCPropertyGridProperty* pGroupInfo =
		new CMFCPropertyGridProperty(_T("회원 정보"));

	// 이름
	CMFCPropertyGridProperty* pName =
		new CMFCPropertyGridProperty(_T("이름"), COleVariant(name), _T("회원 이름"));
	pName->SetData(0);
	pGroupInfo->AddSubItem(pName);

	// 성별
	COleVariant varSex(sex ? _T("남자") : _T("여자"));
	CMFCPropertyGridProperty* pSex =
		new CMFCPropertyGridProperty(_T("성별"), varSex, _T("회원 성별"));
	pSex->AddOption(_T("남자"));
	pSex->AddOption(_T("여자"));
	pSex->SetData(1);
	pGroupInfo->AddSubItem(pSex);

	// 연락처
	CMFCPropertyGridProperty* pContact =
		new CMFCPropertyGridProperty(_T("연락처"), COleVariant(contact), _T("회원 연락처"));
	pContact->SetData(2);
	pGroupInfo->AddSubItem(pContact);

	m_propMember.AddProperty(pGroupInfo);

	//그룹 2: 보기
	CMFCPropertyGridProperty* pGroupView =
		new CMFCPropertyGridProperty(_T("보기"));

	CMFCPropertyGridProperty* pView =
		new CMFCPropertyGridProperty(_T("리스트 뷰"), COleVariant(_T("자세히")));
	pView->AddOption(_T("자세히"));
	pView->AddOption(_T("목록"));
	pView->AddOption(_T("큰 아이콘"));
	pView->SetData(3);

	pGroupView->AddSubItem(pView);

	m_propMember.AddProperty(pGroupView);
}

void CPractice10Dlg::OnClickedButtonAdd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	if (m_strName.IsEmpty() || m_strContact.IsEmpty())
	{
		MessageBox(_T("모든 항목을 입력해 주세요."), _T("잠깐"), MB_OK);
		return;
	}

	int nCount = m_listMembers.GetItemCount();

	CString strNum;
	strNum.Format(_T("%d"), nCount + 1);

	//list control에 항목 추가
	m_listMembers.InsertItem(nCount, strNum);
	m_listMembers.SetItemText(nCount, 1, m_strName);
	m_listMembers.SetItemText(nCount, 2, m_bSex ? _T("남자") : _T("여자"));
	m_listMembers.SetItemText(nCount, 3, m_strContact);
	m_nSelectedMember = nCount;
	//Property Grid 업데이트
	UpdatePropGrid(m_strName, m_bSex, m_strContact);
	//edit 초기화
	m_strName = _T("");
	m_strContact = _T("");
	UpdateData(FALSE);
}

void CPractice10Dlg::OnItemchangedListMembers(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	  // 현재 선택된 행 인덱스
	int index = pNMLV->iItem;

	if (index >= 0 && (pNMLV->uChanged & LVIF_STATE))
	{
		if (pNMLV->uNewState & LVIS_SELECTED)
		{
			m_nSelectedMember = index;
			CString name = m_listMembers.GetItemText(index, 1);
			CString sex = m_listMembers.GetItemText(index, 2);
			CString contact = m_listMembers.GetItemText(index, 3);

			// PropertyGrid를 선택된 회원 정보로 업데이트
			UpdatePropGrid(name, sex == _T("남자"), contact);
		}
	}
	*pResult = 0;
}

void CPractice10Dlg::OnClickedButtonDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_nSelectedMember >= 0)
	{
		// 삭제 확인 메시지
		if (AfxMessageBox(_T("정말로 삭제하시겠습니까?"),
			MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			// 1. 리스트 컨트롤에서 선택된 아이템 삭제
			m_listMembers.DeleteItem(m_nSelectedMember);

			// 2. 아래 있는 아이템들을 위로 이동
			int count = m_listMembers.GetItemCount();
			for (int i = 0; i < count; i++)
			{
				CString strNum;
				strNum.Format(_T("%d"), i + 1);
				m_listMembers.SetItemText(i, 0, strNum);
			}

			// 3. PropertyGrid 초기화
			UpdatePropGrid(_T(""), true, _T(""));

			// 4. 선택된 회원 인덱스 초기화
			m_nSelectedMember = -1;
		}
	}
	else
	{
		// 선택 안 했을 때 메시지
		MessageBox(_T("아이템을 선택하지 않았습니다."),
			_T("잠깐"), MB_OK);
	}
}

void CPractice10Dlg::OnRadioMale()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_bSex = true;
}

void CPractice10Dlg::OnRadioFemale()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_bSex = false;
}

LRESULT CPractice10Dlg::OnPropertyChanged(WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 구현 코드 추가.
	CMFCPropertyGridProperty* pProperty =
		(CMFCPropertyGridProperty*)lParam;

	// 선택된 회원이 없으면 PDF처럼 메시지 출력
	if (m_nSelectedMember < 0)
	{
		MessageBox(_T("아이템을 선택하지 않았습니다."),
			_T("잠깐"), MB_OK);
		return 0L;
	}

	CString strName, strContact, strSex, strView;

	switch (pProperty->GetData())
	{
	case 0:     // 이름
		strName = pProperty->GetValue();
		m_listMembers.SetItemText(m_nSelectedMember, 1, strName);
		break;

	case 1:     // 성별
		strSex = pProperty->GetValue();
		m_listMembers.SetItemText(m_nSelectedMember, 2, strSex);
		break;

	case 2:     // 연락처
		strContact = pProperty->GetValue();
		m_listMembers.SetItemText(m_nSelectedMember, 3, strContact);
		break;

	case 3:     // 보기 변경
		strView = pProperty->GetValue();

		if (strView == _T("자세히"))
		{
			m_listMembers.ModifyStyle(LVS_TYPEMASK, LVS_REPORT);
		}
		else if (strView == _T("목록"))
		{
			m_listMembers.ModifyStyle(LVS_TYPEMASK, LVS_LIST);
		}
		else if (strView == _T("큰 아이콘"))
		{
			m_listMembers.ModifyStyle(LVS_TYPEMASK, LVS_ICON);
		}

		m_listMembers.RedrawWindow();
		break;
	}

	return 0L;
}

void CPractice10Dlg::OnStnClickedMfcpropertygridMember()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
