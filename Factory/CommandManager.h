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
	E_COMMAND_NULL = 0,   //无操作
	E_COMMAND_ADD,		  //新增
	E_COMMAND_DELETE,		//删除
	E_COMMAND_EDIT,			//修改
	E_COMMAND_COPY,			//复制
	E_COMMAND_PASTE,		//黏贴
	E_COMMAND_MOVE,			//移动
	E_COMMAND_CHANGESIZE,	//改变大小
	E_COMMAND_OTHER,		//其他待定
};

class BaseCommand
{
public:
	BaseCommand();
	virtual ~BaseCommand();
	
	virtual bool Execute();				//撤销操作
	virtual bool Unexecute();			//恢复操作
	void			ClearSelectVector();
public:
	vector<CEqmDrawBase *> m_vecBeforeCmd;		//图元操作前备份
	E_COMMAND_TYPE	m_eCmdType;					//图元操作类型
	Gdiplus::PointF prePt;						//移动前位置
	Gdiplus::PointF actPt;						//移动后位置
	Direction		dir;						//改变大小后方向
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

    bool CallCommand(vector<CEqmDrawBase*> vecOperator,E_COMMAND_TYPE eOpType);			//单个元素保存操作
	bool CallCommand(CEqmDrawBase* Operator,E_COMMAND_TYPE eOpType);			//数组元素保存操作
	bool CallMoveCommand(vector<CEqmDrawBase*> vecOperator,Gdiplus::PointF prePt, Gdiplus::PointF actPt);		//移动保存
	bool CallChangeSizeCommand(CEqmDrawBase* Operator,Gdiplus::PointF prePt, Gdiplus::PointF actPt,Direction dir);		//改变大小保存
	void ClearAllCommands();
    void Undo();
    void Redo();
    bool CanUndo() const;
    bool CanRedo() const;
	E_COMMAND_TYPE ChangeTypeUndoToRedo(E_COMMAND_TYPE eType);		//Undo和Redo互转时候 类型替换
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

