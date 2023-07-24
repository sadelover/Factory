#include "StdAfx.h"
#include "CommandManager.h"

#include "../ComponentDraw/EqmDrawBase.h"
#include "../ComponentDraw/EqmDrawDevice.h"
#include "../ComponentDraw/EqmDrawPipe.h"
#include "../ComponentDraw/EqmDrawText.h"
#include "../ComponentDraw/EqmDrawGraphic.h"
#include "../ComponentDraw/EqmDrawGraphicRect.h"
#include "../ComponentDraw/EqmDrawGraphicLine.h"
#include "../ComponentDraw/EqmDrawGraphicEllipse.h"
#include "../ComponentDraw/EqmDrawGraphicPolygon.h"
#include "../ComponentDraw/EqmDrawBend.h"
#include "../ComponentDraw/EqmDrawDashBoard.h"
#include "../ComponentDraw/EqmDrawLineGraphic.h"
#include "../ComponentDraw/EqmDrawBarGraphic.h"
#include "../ComponentDraw/EqmDrawPieGraphic.h"
#include "../ComponentDraw/EqmDrawButton.h"
#include "../ComponentDraw/EqmDrawTimeSelect.h"
#include "../ComponentDraw/EqmDrawTimeBar.h"
#include "../ComponentDraw/EqmDrawProgressBar.h"
#include "../ComponentDraw/EqmDrawDiagnoseGraphic.h"
#include "../ComponentDraw/EqmDrawDiagnoseGraphicPlus.h"
#include "../ComponentDraw/EqmDrawAreaChange.h"
#include "../ComponentDraw/EqmDrawShellExecute.h"
#include "../ComponentDraw/EqmDrawEmbededPage.h"
#include "../ComponentDraw/EqmDrawLiquidLevel.h"
#include "../ComponentDraw/EqmDrawButtonPlus.h"
#include "../ComponentDraw/EqmDrawScrawl.h"
#include "../ComponentDraw/EqmDrawEnergySaveRoi.h"
#include "../ComponentDraw/EqmDrawLevelRuler.h"
#include "../ComponentDraw/CEqmDrawPage.h"
#include "Tools/CustomTools/CustomTools.h"

#include "MainFrm.h"
#include "BEOP-DesignerView.h"
#include "BEOP-designer.h"
#include "PageDlg.h"
#include "CanvasDlg.h"

const int g_nMaxStackSize = 20;			//最大撤销步骤

CommandManager::UndoRedoStateInspector::UndoRedoStateInspector(CommandManager * pCommandManager):
m_pCommandManager(pCommandManager),
m_bUndoable(pCommandManager->CanUndo()),
m_bRedoable(pCommandManager->CanRedo())
{
}

CommandManager::UndoRedoStateInspector::~UndoRedoStateInspector()
{
    if (m_bUndoable != m_pCommandManager->CanUndo() || m_bRedoable != m_pCommandManager->CanRedo())
    {
       
    }
}

CommandManager::CommandManager()
{
}

CommandManager::~CommandManager()
{
    ClearAllCommands();
}

CommandManager * CommandManager::Instance()
{
    return Singleton<CommandManager>::Instance();
}

bool CommandManager::CallCommand(vector<CEqmDrawBase*> vecOperator,E_COMMAND_TYPE eOpType)
{
    UndoRedoStateInspector si(this);

	if(vecOperator.size()>0 && eOpType>E_COMMAND_NULL)
	{
		BaseCommand* pCommand = new BaseCommand();
		pCommand->m_eCmdType = eOpType;
		for(int i=0; i<vecOperator.size(); ++i)
		{
			if(vecOperator[i])		
			{
				CEqmDrawBase* pBase = vecOperator[i]->Copy();
				if(pBase)
				{
					pCommand->m_vecBeforeCmd.push_back(pBase);
				}
			}
		}
		PushUndoCommand(pCommand);
		DeleteRedoCommands();
		return true;
	}

    return false;
}

bool CommandManager::CallCommand( CEqmDrawBase* Operator,E_COMMAND_TYPE eOpType )
{
	UndoRedoStateInspector si(this);

	if(Operator != NULL && eOpType>E_COMMAND_NULL)
	{
		BaseCommand* pCommand = new BaseCommand();
		pCommand->m_eCmdType = eOpType;

		CEqmDrawBase* pBase = Operator->Copy();
		if(pBase)
		{
			pCommand->m_vecBeforeCmd.push_back(pBase);
		}
		PushUndoCommand(pCommand);
		DeleteRedoCommands();
		return true;
	}

	return false;
}

void CommandManager::ClearAllCommands()
{
    UndoRedoStateInspector si(this);

    DeleteUndoCommands();
    DeleteRedoCommands();
}

void CommandManager::Undo()
{
    UndoRedoStateInspector si(this);

    BaseCommand * pCommand = PopUndoCommand();
    if (pCommand)
    {
		BaseCommand* pCommandCopy = new BaseCommand();
		pCommandCopy->m_eCmdType = ChangeTypeUndoToRedo(pCommand->m_eCmdType);
		if(pCommand->m_eCmdType == E_COMMAND_MOVE)			//移动
		{
			pCommandCopy->prePt = pCommand->actPt;
			pCommandCopy->actPt = pCommand->prePt;
		}
		if(pCommand->m_eCmdType == E_COMMAND_CHANGESIZE)	//改变大小
		{
			pCommandCopy->prePt = pCommand->actPt;
			pCommandCopy->actPt = pCommand->prePt;
			pCommandCopy->dir = pCommand->dir;
		}
		if(pCommand->m_eCmdType == E_COMMAND_DELETE)//上一步是删除
		{
			for(int i=0; i<pCommand->m_vecBeforeCmd.size(); ++i)
			{
				if(pCommand->m_vecBeforeCmd[i])
				{
					CEqmDrawBase* pElement = pCommand->m_vecBeforeCmd[i]->Copy();
					if(pElement)
					{
						pCommandCopy->m_vecBeforeCmd.push_back(pElement);
					}
				}
			}
		}
		else
		{
			CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
			for(int i=0; i<pCommand->m_vecBeforeCmd.size(); ++i)
			{
				if(pCommand->m_vecBeforeCmd[i])
				{
					CEqmDrawPage* pPage = pMainFrame->m_project.GetPageByID(pCommand->m_vecBeforeCmd[i]->GetPageID());
					if(pPage)
					{
						CEqmDrawBase* pElement = pPage->GetElementBase(pCommand->m_vecBeforeCmd[i]->GetEqmType(),pCommand->m_vecBeforeCmd[i]->GetID());
						if(pElement)
						{
							CEqmDrawBase* pBase = pElement->Copy();
							if(pBase)
							{
								pCommandCopy->m_vecBeforeCmd.push_back(pBase);
							}
						}
					}			
				}
			}
		}
		
        if (pCommand->Unexecute())
        {
            PushRedoCommand(pCommandCopy);
        }
		delete pCommand;
    }
}

void CommandManager::Redo()
{
    UndoRedoStateInspector si(this);

    BaseCommand * pCommand = PopRedoCommand();
	if (pCommand)
	{
		BaseCommand* pCommandCopy = new BaseCommand();
		pCommandCopy->m_eCmdType = ChangeTypeUndoToRedo(pCommand->m_eCmdType);
		if(pCommand->m_eCmdType == E_COMMAND_MOVE)
		{
			pCommandCopy->prePt = pCommand->actPt;
			pCommandCopy->actPt = pCommand->prePt;
		}
		if(pCommand->m_eCmdType == E_COMMAND_CHANGESIZE)	//改变大小
		{
			pCommandCopy->prePt = pCommand->actPt;
			pCommandCopy->actPt = pCommand->prePt;
			pCommandCopy->dir = pCommand->dir;
		}
		if(pCommand->m_eCmdType == E_COMMAND_DELETE || pCommand->m_eCmdType == E_COMMAND_PASTE)
		{
			for(int i=0; i<pCommand->m_vecBeforeCmd.size(); ++i)
			{
				if(pCommand->m_vecBeforeCmd[i])
				{
					CEqmDrawBase* pElement = pCommand->m_vecBeforeCmd[i]->Copy();
					pCommandCopy->m_vecBeforeCmd.push_back(pElement);
				}
			}
		}
		else
		{
			CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
			for(int i=0; i<pCommand->m_vecBeforeCmd.size(); ++i)
			{
				if(pCommand->m_vecBeforeCmd[i])
				{
					CEqmDrawPage* pPage = pMainFrame->m_project.GetPageByID(pCommand->m_vecBeforeCmd[i]->GetPageID());
					if(pPage)
					{
						CEqmDrawBase* pElement = pPage->GetElementBase(pCommand->m_vecBeforeCmd[i]->GetEqmType(),pCommand->m_vecBeforeCmd[i]->GetID());
						pElement->SetDeleteFlag(true);
						if(pElement)
						{
							CEqmDrawBase* pBase = pElement->Copy();
							if(pBase)
							{								
								pCommandCopy->m_vecBeforeCmd.push_back(pBase);
							}
						}
					}			
				}
			}
		}

		if (pCommand->Execute())
		{
			PushUndoCommand(pCommandCopy);
		}
		delete pCommand;
	}
}

bool CommandManager::CanUndo() const
{
    return m_stackUndo.size();
}

bool CommandManager::CanRedo() const
{
    return m_stackRedo.size();
}

void CommandManager::PushUndoCommand(BaseCommand * pCommand)
{
    if (pCommand)
    {
		if(m_stackUndo.size()>=g_nMaxStackSize)
		{
			delete m_stackUndo[0];
			m_stackUndo[0] = NULL;
			m_stackUndo.erase(m_stackUndo.begin());
		}
        m_stackUndo.push_back(pCommand);
    }
}

BaseCommand * CommandManager::PopUndoCommand()
{
    BaseCommand * pCommand = NULL;
    if (m_stackUndo.size()>0)
    {
        pCommand = m_stackUndo[m_stackUndo.size()-1];
		m_stackUndo.pop_back();
    }
    return pCommand;
}

void CommandManager::PushRedoCommand(BaseCommand * pCommand)
{
	if (pCommand)
	{
		if(m_stackRedo.size()>=g_nMaxStackSize)
		{
			delete m_stackRedo[0];
			m_stackRedo[0] = NULL;
			m_stackRedo.erase(m_stackRedo.begin());
		}
		m_stackRedo.push_back(pCommand);
	}
}

BaseCommand * CommandManager::PopRedoCommand()
{
	BaseCommand * pCommand = NULL;
	if (m_stackRedo.size()>0)
	{
		pCommand = m_stackRedo[m_stackRedo.size()-1];
		m_stackRedo.pop_back();
	}
	return pCommand;
}

void CommandManager::DeleteUndoCommands()
{
	for(int i=0; i<m_stackUndo.size(); ++i)
	{
		if(m_stackUndo[i])
		{
			delete m_stackUndo[i];
			m_stackUndo[i] = NULL;
		}
	}
	m_stackUndo.clear();
}

void CommandManager::DeleteRedoCommands()
{
	for(int i=0; i<m_stackRedo.size(); ++i)
	{
		if(m_stackRedo[i])
		{
			delete m_stackRedo[i];
			m_stackRedo[i] = NULL;
		}
	}
	m_stackRedo.clear();
}

E_COMMAND_TYPE CommandManager::ChangeTypeUndoToRedo( E_COMMAND_TYPE eType)
{
	switch(eType)
	{
	case E_COMMAND_ADD:
		eType = E_COMMAND_DELETE;
		break;
	case E_COMMAND_DELETE:
		eType = E_COMMAND_ADD;
		break;
	default:
		break;
	}
	return eType;
}

bool CommandManager::CallMoveCommand( vector<CEqmDrawBase*> vecOperator,Gdiplus::PointF prePt, Gdiplus::PointF actPt )
{
	UndoRedoStateInspector si(this);

	if(vecOperator.size()>0 && (prePt.X != actPt.X || prePt.Y != actPt.Y))
	{
		BaseCommand* pCommand = new BaseCommand();
		pCommand->m_eCmdType = E_COMMAND_MOVE;
		for(int i=0; i<vecOperator.size(); ++i)
		{
			if(vecOperator[i])		
			{
				CEqmDrawBase* pBase = vecOperator[i]->Copy();
				if(pBase)
				{
					pCommand->m_vecBeforeCmd.push_back(pBase);
				}
			}
		}
		pCommand->prePt = prePt;
		pCommand->actPt = actPt;
		PushUndoCommand(pCommand);
		DeleteRedoCommands();
		return true;
	}

	return false;
}

bool CommandManager::CallChangeSizeCommand( CEqmDrawBase* Operator,Gdiplus::PointF prePt, Gdiplus::PointF actPt,Direction dir )
{
	UndoRedoStateInspector si(this);

	if(Operator && (prePt.X != actPt.X || prePt.Y != actPt.Y) && dir > Dir_None)
	{
		BaseCommand* pCommand = new BaseCommand();
		pCommand->m_eCmdType = E_COMMAND_CHANGESIZE;

		CEqmDrawBase* pBase = Operator->Copy();
		if(pBase)
		{
			pCommand->m_vecBeforeCmd.push_back(pBase);
		}
		pCommand->actPt = prePt;
		pCommand->prePt = actPt;
		pCommand->dir = dir;
		PushUndoCommand(pCommand);
		DeleteRedoCommands();
		return true;
	}

	return false;
}

void BaseCommand::ClearSelectVector()
{
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT(pMainFrame);
	for(int i=0; i<pMainFrame->m_DrawContext.vecSelectEqmt.size(); ++i)
	{
		if(pMainFrame->m_DrawContext.vecSelectEqmt[i])
		{
			pMainFrame->m_DrawContext.vecSelectEqmt[i]->SetSelected(false);
		}
	}
	pMainFrame->m_DrawContext.vecSelectEqmt.clear();
	pMainFrame->m_wndProperties.SetNullProperty();
}

bool BaseCommand::Unexecute()
{
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT(pMainFrame);
	CBEOPDesignerView* pView = static_cast<CBEOPDesignerView*>(pMainFrame->GetActiveView());
	ClearSelectVector();
	switch(m_eCmdType)
	{
	case E_COMMAND_ADD:				//增加操作->删除
		{
			for(int i=0; i<m_vecBeforeCmd.size(); ++i)
			{
				CEqmDrawBase* pBase = m_vecBeforeCmd[i];
				if(pBase)
				{		
					CEqmDrawPage* pPage = pMainFrame->m_project.GetPageByID(pBase->GetPageID());
					if(pPage)
					{
						CEqmDrawBase* pElement = pPage->GetElementBase(pBase->GetEqmType(),pBase->GetID());
						if(pElement)
						{
							pElement->SetDeleteFlag(true);
							pElement->SetNeedSave(true);
						}
					}					
				}
			}
			pMainFrame->m_wndProperties.SetNullProperty();
			pMainFrame->m_DrawContext.m_bStartChangeSize = false;
			pMainFrame->m_DrawContext.m_bIsChangeSize    = false;
			pMainFrame->m_DrawContext.m_nDrawMode = EQMDRAW_MOVE;
		}
		break;
	case E_COMMAND_DELETE:			//删除操作
		{
			for(int i=0; i<m_vecBeforeCmd.size(); ++i)
			{
				CEqmDrawBase* pBase = m_vecBeforeCmd[i];
				if(pBase)
				{		
					CEqmDrawPage* pPage = pMainFrame->m_project.GetPageByID(pBase->GetPageID());
					if(pPage)
					{
						CEqmDrawBase* pElement = pPage->GetElementBase(pBase->GetEqmType(),pBase->GetID());
						if(pElement)
						{
							pElement->SetSelected(false);
							pElement->SetDeleteFlag(false);
							pElement->SetNeedSave(true);
						}
					}
				}
			}
			pMainFrame->m_DrawContext.m_bStartChangeSize = false;
			pMainFrame->m_DrawContext.m_bIsChangeSize    = false;
			pMainFrame->m_DrawContext.m_nDrawMode = EQMDRAW_MOVE;
		}
		break;
	case E_COMMAND_EDIT:				//修改操作
		{
			for(int i=0; i<m_vecBeforeCmd.size(); ++i)
			{
				CEqmDrawBase* pBase = m_vecBeforeCmd[i];
				if(pBase)
				{		
					CEqmDrawPage* pPage = pMainFrame->m_project.GetPageByID(pBase->GetPageID());
					if(pPage)
					{
						CEqmDrawBase* pElement = pPage->GetElementBase(pBase->GetEqmType(),pBase->GetID());
						if(pElement)
						{
							pElement->ClonePropertyByDest(pBase);
							if(pElement->GetEqmType() == EQMDRAW_TEXT)
							{
								CEqmDrawText* pText = (CEqmDrawText*)pElement;
								pText->DeleteFont();
								pText->DeleteSolidBrush();
								pText->DeleteHatchBrush();
								pText->DeleteCommonBorderPen();
								pText->Delete3DPenSunken();
								pText->Delete3DPenHump();
							}
							pElement->SetSelected(false);
							pElement->SetNeedSave(true);
						}
					}
				}
			}
			pMainFrame->m_DrawContext.m_bStartChangeSize = false;
			pMainFrame->m_DrawContext.m_bIsChangeSize    = false;
			pMainFrame->m_DrawContext.m_nDrawMode = EQMDRAW_MOVE;
		}
		break;
	case E_COMMAND_COPY:				//复制操作
		{
			pMainFrame->m_DrawContext.m_bStartChangeSize = false;
			pMainFrame->m_DrawContext.m_bIsChangeSize    = false;
			pMainFrame->m_DrawContext.m_nDrawMode = EQMDRAW_MOVE;
			pMainFrame->m_DrawContext.mVecSelectEqmtCopied.clear();
		}
		break;
	case E_COMMAND_PASTE:				//黏贴操作
		{
			for(int i=0; i<m_vecBeforeCmd.size(); ++i)
			{
				CEqmDrawBase* pBase = m_vecBeforeCmd[i];
				if(pBase)
				{		
					CEqmDrawPage* pPage = pMainFrame->m_project.GetPageByID(pBase->GetPageID());
					if(pPage)
					{
						CEqmDrawBase* pElement = pPage->GetElementBase(pBase->GetEqmType(),pBase->GetID());
						if(pElement)
						{
							pMainFrame->m_pPrjSqlite->DeleteElement(pElement->GetEqmType(), pElement->GetPageID(),pElement->GetID());
							pPage->DeleteBase(pElement);
						}
					}
				}
			}
			pMainFrame->m_wndProperties.SetNullProperty();
			pMainFrame->m_DrawContext.m_bStartChangeSize = false;
			pMainFrame->m_DrawContext.m_bIsChangeSize    = false;
			pMainFrame->m_DrawContext.m_nDrawMode = EQMDRAW_MOVE;
		}
		break;
	case E_COMMAND_MOVE:					//移动
		{
			for(int i=0; i<m_vecBeforeCmd.size(); ++i)
			{
				CEqmDrawBase* pBase = m_vecBeforeCmd[i];
				if(pBase)
				{		
					CEqmDrawPage* pPage = pMainFrame->m_project.GetPageByID(pBase->GetPageID());
					if(pPage)
					{
						CEqmDrawBase* pElement = pPage->GetElementBase(pBase->GetEqmType(),pBase->GetID());
						if(pElement)
						{
							pElement->SetSelected(false);
							pElement->DoMoveEqmt(actPt,prePt);
						}
					}
				}
			}
			pMainFrame->m_DrawContext.m_bStartChangeSize = false;
			pMainFrame->m_DrawContext.m_bIsChangeSize    = false;
			pMainFrame->m_DrawContext.m_nDrawMode = EQMDRAW_MOVE;
		}
		break;
	case E_COMMAND_CHANGESIZE:					//改变大小
		{
			for(int i=0; i<m_vecBeforeCmd.size(); ++i)
			{
				CEqmDrawBase* pBase = m_vecBeforeCmd[i];
				if(pBase)
				{		
					CEqmDrawPage* pPage = pMainFrame->m_project.GetPageByID(pBase->GetPageID());
					if(pPage)
					{
						CEqmDrawBase* pElement = pPage->GetElementBase(pBase->GetEqmType(),pBase->GetID());
						if(pElement)
						{
							pElement->SetSelected(false);
							//pElement->SetPos(prePt);
							pElement->ChangeSize(actPt,dir);
						}
					}
				}
			}
			pMainFrame->m_DrawContext.m_bStartChangeSize = false;
			pMainFrame->m_DrawContext.m_bIsChangeSize    = false;
			pMainFrame->m_DrawContext.m_nDrawMode = EQMDRAW_MOVE;
		}
		break;
	default:
		break;
	}
	pView->m_PageDlg->m_dlgCanvas.Invalidate(false);
	return true;
}

bool BaseCommand::Execute()
{
	CMainFrame *pMainFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT(pMainFrame);
	CBEOPDesignerView* pView = static_cast<CBEOPDesignerView*>(pMainFrame->GetActiveView());
	ClearSelectVector();
	switch(m_eCmdType)
	{
	case E_COMMAND_ADD:				//增加操作->删除
		{
			for(int i=0; i<m_vecBeforeCmd.size(); ++i)
			{
				CEqmDrawBase* pBase = m_vecBeforeCmd[i];
				if(pBase)
				{		
					CEqmDrawPage* pPage = pMainFrame->m_project.GetPageByID(pBase->GetPageID());
					if(pPage)
					{
						CEqmDrawBase* pElement = pPage->GetElementBase(pBase->GetEqmType(),pBase->GetID());
						if(pElement)
						{
							pElement->SetDeleteFlag(true);
						}
					}
				}
			}
			pMainFrame->m_wndProperties.SetNullProperty();
			pMainFrame->m_DrawContext.m_bStartChangeSize = false;
			pMainFrame->m_DrawContext.m_bIsChangeSize    = false;
			pMainFrame->m_DrawContext.m_nDrawMode = EQMDRAW_MOVE;
		}
		break;
	case E_COMMAND_DELETE:			//删除操作
		{
			for(int i=0; i<m_vecBeforeCmd.size(); ++i)
			{
				CEqmDrawBase* pBase = m_vecBeforeCmd[i];
				if(pBase)
				{		
					CEqmDrawPage* pPage = pMainFrame->m_project.GetPageByID(pBase->GetPageID());
					if(pPage)
					{
						CEqmDrawBase* pElement = pPage->GetElementBase(pBase->GetEqmType(),pBase->GetID());
						pElement->SetSelected(false);
						pElement->SetDeleteFlag(false);
					}
				}
			}
			pMainFrame->m_DrawContext.m_bStartChangeSize = false;
			pMainFrame->m_DrawContext.m_bIsChangeSize    = false;
			pMainFrame->m_DrawContext.m_nDrawMode = EQMDRAW_MOVE;
		}
		break;
	case E_COMMAND_EDIT:				//修改操作
		{
			for(int i=0; i<m_vecBeforeCmd.size(); ++i)
			{
				CEqmDrawBase* pBase = m_vecBeforeCmd[i];
				if(pBase)
				{		
					CEqmDrawPage* pPage = pMainFrame->m_project.GetPageByID(pBase->GetPageID());
					if(pPage)
					{
						CEqmDrawBase* pElement = pPage->GetElementBase(pBase->GetEqmType(),pBase->GetID());
						if(pElement)
						{
							pElement->ClonePropertyByDest(pBase);
							if(pElement->GetEqmType() == EQMDRAW_TEXT)
							{
								CEqmDrawText* pText = (CEqmDrawText*)pElement;
								pText->DeleteFont();
								pText->DeleteSolidBrush();
								pText->DeleteHatchBrush();
								pText->DeleteCommonBorderPen();
								pText->Delete3DPenSunken();
								pText->Delete3DPenHump();
							}
							pElement->SetSelected(false);
							pElement->SetNeedSave(true);
						}
					}
				}
			}
			pMainFrame->m_DrawContext.m_bStartChangeSize = false;
			pMainFrame->m_DrawContext.m_bIsChangeSize    = false;
			pMainFrame->m_DrawContext.m_nDrawMode = EQMDRAW_MOVE;
		}
		break;
	case E_COMMAND_COPY:				//复制操作
		{
			pMainFrame->m_DrawContext.mVecSelectEqmtCopied.clear();
			for(int i=0; i<m_vecBeforeCmd.size(); ++i)
			{
				CEqmDrawBase* pBase = m_vecBeforeCmd[i];
				if(pBase)
				{		
					CEqmDrawPage* pPage = pMainFrame->m_project.GetPageByID(pBase->GetPageID());
					if(pPage)
					{
						CEqmDrawBase* pElement = pPage->GetElementBase(pBase->GetEqmType(),pBase->GetID());
						if(pElement)
						{
							pMainFrame->m_DrawContext.mVecSelectEqmtCopied.push_back(pElement);
						}
					}
				}
			}

			pMainFrame->m_DrawContext.m_bStartChangeSize = false;
			pMainFrame->m_DrawContext.m_bIsChangeSize    = false;
			pMainFrame->m_DrawContext.m_nDrawMode = EQMDRAW_MOVE;
		}
		break;
	case E_COMMAND_PASTE:				//黏贴操作
		{
			pMainFrame->m_DrawContext.vecSelectEqmt.clear();
			for(int i=0; i<m_vecBeforeCmd.size(); ++i)
			{
				CEqmDrawBase* pBase = m_vecBeforeCmd[i];
				CEqmDrawBase* pInsert = pBase->Copy();
				if(pInsert)
				{		
					CEqmDrawPage* pPage = pMainFrame->m_project.GetPageByID(pInsert->GetPageID());
					if(pPage)
					{
						pInsert->SetSelected(false);
						pPage->AddBase(pInsert);
					}
				}
			}
			pMainFrame->m_DrawContext.m_bStartChangeSize = false;
			pMainFrame->m_DrawContext.m_bIsChangeSize    = false;
			pMainFrame->m_DrawContext.m_nDrawMode = EQMDRAW_MOVE;
		}
		break;
	case E_COMMAND_MOVE:					//移动
		{
			for(int i=0; i<m_vecBeforeCmd.size(); ++i)
			{
				CEqmDrawBase* pBase = m_vecBeforeCmd[i];
				if(pBase)
				{		
					CEqmDrawPage* pPage = pMainFrame->m_project.GetPageByID(pBase->GetPageID());
					if(pPage)
					{
						CEqmDrawBase* pElement = pPage->GetElementBase(pBase->GetEqmType(),pBase->GetID());
						if(pElement)
						{
							pElement->SetSelected(false);
							pElement->DoMoveEqmt(actPt,prePt);
						}
					}
				}
			}
			pMainFrame->m_DrawContext.m_bStartChangeSize = false;
			pMainFrame->m_DrawContext.m_bIsChangeSize    = false;
			pMainFrame->m_DrawContext.m_nDrawMode = EQMDRAW_MOVE;
		}
		break;
	case E_COMMAND_CHANGESIZE:					//改变大小
		{
			for(int i=0; i<m_vecBeforeCmd.size(); ++i)
			{
				CEqmDrawBase* pBase = m_vecBeforeCmd[i];
				if(pBase)
				{		
					CEqmDrawPage* pPage = pMainFrame->m_project.GetPageByID(pBase->GetPageID());
					if(pPage)
					{
						CEqmDrawBase* pElement = pPage->GetElementBase(pBase->GetEqmType(),pBase->GetID());
						if(pElement)
						{
							pElement->SetSelected(false);
							//pElement->SetPos(prePt);
							pElement->ChangeSize(actPt,dir);
						}
					}
				}
			}
			pMainFrame->m_DrawContext.m_bStartChangeSize = false;
			pMainFrame->m_DrawContext.m_bIsChangeSize    = false;
			pMainFrame->m_DrawContext.m_nDrawMode = EQMDRAW_MOVE;
		}
		break;
	default:
		break;
	}
	pView->m_PageDlg->m_dlgCanvas.Invalidate(false);
	return true;
}

BaseCommand::~BaseCommand()
{
	for(int i=0; i<m_vecBeforeCmd.size(); ++i)
	{
		delete m_vecBeforeCmd[i];
		m_vecBeforeCmd[i] = NULL;
	}
}

BaseCommand::BaseCommand()
{
	dir = Dir_None;
	actPt.X = 0;
	actPt.Y = 0;
	prePt.X = 0;
	prePt.Y = 0;
}
