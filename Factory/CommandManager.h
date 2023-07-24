#pragma once
#include <vector>
#include "../ComponentDraw/EqmDrawDefines.h"
#define EVENT_UNDOREDOSTATECHANGED  1
class CEqmDrawBase;

template <typename T>
class Singleton
{
private:
	Singleton() {}
	Singleton(const Singleton& rhs);
	Singleton& operator=(const Singleton& rhs);

public:
	static T * Instance()
	{
		static T * pT = NULL;
		if (!pT)
		{
			static T instance;
			pT = &instance;
		} 
		return pT;
	}
};

enum E_COMMAND_TYPE
{
	E_COMMAND_NULL = 0,   //�޲���
	E_COMMAND_ADD,		  //����
	E_COMMAND_DELETE,		//ɾ��
	E_COMMAND_EDIT,			//�޸�
	E_COMMAND_COPY,			//����
	E_COMMAND_PASTE,		//���
	E_COMMAND_MOVE,			//�ƶ�
	E_COMMAND_CHANGESIZE,	//�ı��С
	E_COMMAND_OTHER,		//��������
};

class BaseCommand
{
public:
	BaseCommand();
	virtual ~BaseCommand();
	
	virtual bool Execute();				//��������
	virtual bool Unexecute();			//�ָ�����
	void			ClearSelectVector();
public:
	vector<CEqmDrawBase *> m_vecBeforeCmd;		//ͼԪ����ǰ����
	E_COMMAND_TYPE	m_eCmdType;					//ͼԪ��������
	Gdiplus::PointF prePt;						//�ƶ�ǰλ��
	Gdiplus::PointF actPt;						//�ƶ���λ��
	Direction		dir;						//�ı��С����
};

class CommandManager
{
    class UndoRedoStateInspector
    {
        friend class CommandManager;

    private:
        UndoRedoStateInspector(CommandManager * pCommandManager);
        ~UndoRedoStateInspector();

    private:
        CommandManager * m_pCommandManager;
        bool m_bUndoable;
        bool m_bRedoable;
    };
	 friend class Singleton<CommandManager>;
private:
    CommandManager();
	~CommandManager();
    CommandManager(const CommandManager& rhs);
    CommandManager& operator=(const CommandManager& rhs);

public:
    static CommandManager * Instance();

    bool CallCommand(vector<CEqmDrawBase*> vecOperator,E_COMMAND_TYPE eOpType);			//����Ԫ�ر������
	bool CallCommand(CEqmDrawBase* Operator,E_COMMAND_TYPE eOpType);			//����Ԫ�ر������
	bool CallMoveCommand(vector<CEqmDrawBase*> vecOperator,Gdiplus::PointF prePt, Gdiplus::PointF actPt);		//�ƶ�����
	bool CallChangeSizeCommand(CEqmDrawBase* Operator,Gdiplus::PointF prePt, Gdiplus::PointF actPt,Direction dir);		//�ı��С����
	void ClearAllCommands();
    void Undo();
    void Redo();
    bool CanUndo() const;
    bool CanRedo() const;
	E_COMMAND_TYPE ChangeTypeUndoToRedo(E_COMMAND_TYPE eType);		//Undo��Redo��תʱ�� �����滻
private:
    void			PushUndoCommand(BaseCommand * pCommand);
    BaseCommand *	PopUndoCommand();
    void			PushRedoCommand(BaseCommand * pCommand);
    BaseCommand *	PopRedoCommand();
    void			DeleteUndoCommands();
    void			DeleteRedoCommands();
private:
    std::vector<BaseCommand *> m_stackUndo;
    std::vector<BaseCommand *> m_stackRedo;
};

