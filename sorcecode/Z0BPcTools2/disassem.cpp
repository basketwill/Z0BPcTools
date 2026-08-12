#include "stdafx.h"
#include "disassem.h"
#include <strsafe.h>
#include "util.h"


CZ0BTDisasm::CZ0BTDisasm()
	:ud_moudle_base(0)
	,ud_moudle_size(0)
	,ud_relove_skip_jmp(NULL)
{
	ud_pView = NULL;
#ifdef _WIN64	

	ud_init(&ud_obj);
	ud_set_mode(&ud_obj, 64);
	ud_set_syntax(&ud_obj, UD_SYN_INTEL);

#else

	/* initialize */
	ud_init(&ud_obj);
	ud_set_mode(&ud_obj, 32);
	ud_set_syntax(&ud_obj, UD_SYN_INTEL);

#endif
	
}

CZ0BTDisasm::~CZ0BTDisasm()
{
	if (ud_pView)
	{
		delete ud_pView;
		ud_pView = NULL;
	}
}

void CZ0BTDisasm::SetMoudleInfo( uint64_t moudle_base,uint64_t moudle_size)
{
	ud_moudle_base = moudle_base;
	ud_moudle_size = moudle_size;
}

void CZ0BTDisasm::SetMode(int mode)
{
	ud_set_mode(&ud_obj,mode);
}

void CZ0BTDisasm::SetResolverSymbol(const LPTSTR (*resolver)(struct ud*, 
	uint64_t addr,
	int64_t *offset),
	void* userdata)
{
	ud_set_sym_resolver(&ud_obj,resolver);
	ud_set_user_opaque_data(&ud_obj,userdata);
}

void CZ0BTDisasm::SetResolverSkipjmp(
				void* (*relove_skip_jmp)( struct ud& ud_obj , void* ,uint64_t,int),void* userdata)
{
	ud_relove_skip_jmp = relove_skip_jmp;
	ud_obj.user_opaque_data = userdata;
}

BOOL CZ0BTDisasm::DisasmBuffer( 
			LPBYTE pBuffer,
			uint64_t BufSize,
			uint64_t Offset,
			BOOL bIsX64,
			LPTSTR lpSectionName
			)
{
	if (!pBuffer ||
		!BufSize)
	{
		return FALSE;
	}

	if (ud_pView)
	{
		delete ud_pView;
		ud_pView = NULL;
	}

	ud_pView = new CViewData();

	if (!ud_pView)
	{
		return FALSE;
	}

	//BOOL bIsX64 = (ud_obj.dis_mode == 64);
	//ud_init(&ud_obj);
	ud_set_input_buffer(&ud_obj, pBuffer, BufSize);
	ud_set_pc(&ud_obj,(uint64_t)Offset);
	ud_set_mode(&ud_obj, 32);
	if (bIsX64)
	{
		ud_set_mode(&ud_obj, 64);
	}
	CViewData* pView = ud_pView;

	

	if (ud_disassemble(&ud_obj) != 0)
	{
		tagOption* Option = NULL;
		// printf("\t%s\n", ud_insn_asm(&ud_obj));
		int nSize = 0;
		if (ParseAsm(ud_obj,Option,nSize) && nSize)
		{
			TCHAR szAddress[64] = {0};

			// 			if (ud_insn_off(&ud_obj) == 0x11500)
			// 			{
			// 				int i = 0;
			// 				i++;
			// 			}
			if (lpSectionName)
			{
				if (bIsX64)
				{
					StringCchPrintf( szAddress,64,_T(" %s:%016llX"),lpSectionName,ud_insn_off(&ud_obj));
				}
				else
				{
					StringCchPrintf( szAddress,64,_T(" %s:%08X"),lpSectionName,ud_insn_off(&ud_obj));
				}

			}
			else
			{
				if (bIsX64)
				{
					StringCchPrintf( szAddress,64,_T(" %016llX"),ud_insn_off(&ud_obj));
				}
				else
				{
					StringCchPrintf( szAddress,64,_T(" %08X"),ud_insn_off(&ud_obj));
				}

			}

			LPVOID pItem = pView->Insert(ud_insn_off(&ud_obj),szAddress);

			if (pItem)
			{
				switch (ud_obj.mnemonic)
				{
				case UD_Ijo:
				case UD_Ijno:
				case UD_Ijb:
				case UD_Ijae:
				case UD_Ijz:
				case UD_Ijnz:
				case UD_Ijbe:
				case UD_Ija:
				case UD_Ijs:
				case UD_Ijns:
				case UD_Ijp:
				case UD_Ijnp:
				case UD_Ijl:
				case UD_Ijge:
				case UD_Ijle:
				case UD_Ijg:
				case UD_Ijcxz:
				case UD_Ijecxz:
				case UD_Ijrcxz:
				case UD_Ijmp:
					{
						pView->SetInstructionType(kInstructionJmp);
						if ( ud_obj.operand[0]._legacy >= ud_moudle_base &&
							ud_obj.operand[0]._legacy <= (ud_moudle_base + ud_moudle_size) )
						{
							if (ud_relove_skip_jmp)
							{
								//ud_relove_skip_jmp( ud_obj, pItem ,ud_obj.operand[0]._legacy,kJmpOther);
							}
						}
					}
					break;
				case UD_Icall:
					{
						pView->SetInstructionType(kInstructionCall);
						if ( ud_obj.operand[0]._legacy >= ud_moudle_base &&
							ud_obj.operand[0]._legacy <= (ud_moudle_base + ud_moudle_size) )
						{
							if (ud_relove_skip_jmp)
							{
								//ud_relove_skip_jmp( ud_obj, pItem ,ud_obj.operand[0]._legacy,kCallFar);
							}
						}
					}
					break;
				case UD_Iret:
				case UD_Iiretw:
				case UD_Iiretd:
				case UD_Iiretq:
					{
						pView->SetInstructionType(kInstructionRet);

						//	if ( ud_obj.operand[0]._legacy >= ud_moudle_base &&
						//		ud_obj.operand[0]._legacy <= (ud_moudle_base + ud_moudle_size) )
						{
							if (ud_relove_skip_jmp)
							{
							//	ud_relove_skip_jmp( ud_obj, pItem ,ud_obj.operand[0]._legacy,kRetType);
							}
						}
					}
					break;
				case UD_Imov:
				case UD_Imovapd:
				case UD_Imovaps:
				case UD_Imovd:
				case UD_Imovhpd:
				case UD_Imovhps:
				case UD_Imovlhps:
				case UD_Imovlpd:
				case UD_Imovlps:
				case UD_Imovhlps:
				case UD_Imovmskpd:
				case UD_Imovmskps:
				case UD_Imovntdq:
				case UD_Imovnti:
				case UD_Imovntpd:
				case UD_Imovntps:
				case UD_Imovntq:
				case UD_Imovq:
				case UD_Imovsb:
				case UD_Imovsw:
				case UD_Imovsd:
				case UD_Imovsq:
				case UD_Imovss:
				case UD_Imovsx:
				case UD_Imovupd:
				case UD_Imovups:
				case UD_Imovzx:
					{
						pView->SetInstructionType(kInstructionMov);
					}
					break;
				case UD_Iint:
					{
						pView->SetInstructionType(kInstructionInt);
					}
					break;
				case UD_Iint1:
					{
						pView->SetInstructionType(kInstructionInt1);
					}
					break;
				case UD_Iint3:
					{
						pView->SetInstructionType(kInstructionInt3);
					}
					break;
				case UD_Iadd:
					{
						if (ud_obj.inp_ctr == 2)
						{
							if ( ud_obj.insn_hexcode[0] == '0' &&
								ud_obj.insn_hexcode[1] == '0' &&
								ud_obj.insn_hexcode[2] == '0' &&
								ud_obj.insn_hexcode[3] == '0' )
							{
								pView->SetInstructionType(kInstruction0000);
							}
						}
					}
					break;
				case UD_Iinvalid:
					{
						pView->SetInstructionType(kInstructionInvalid);
					}
					break;
				default:
					break;
				}
			}

			StringCchPrintf( szAddress,64,_T(" %s"),ud_insn_hex(&ud_obj));
			pView->InsertHex(4,szAddress);

			if( ud_relove_skip_jmp)
			{
				//ud_relove_skip_jmp( ud_obj, pItem ,0,0);
			}

			pView->SetViewColumn(1,0,Option,nSize);

			switch (ud_obj.mnemonic)
			{
			case UD_Ijo:
			case UD_Ijno:
			case UD_Ijb:
			case UD_Ijae:
			case UD_Ijz:
			case UD_Ijnz:
			case UD_Ijbe:
			case UD_Ija:
			case UD_Ijs:
			case UD_Ijns:
			case UD_Ijp:
			case UD_Ijnp:
			case UD_Ijl:
			case UD_Ijge:
			case UD_Ijle:
			case UD_Ijg:
			case UD_Ijcxz:
			case UD_Ijecxz:
			case UD_Ijrcxz:
			case UD_Ijmp:
				{
					pView->SetHexIcon(3,JMP_OTHER_BODY_FLAGS,0,0);
				}
			case UD_Icall:
				{
					pView->SetHexIcon(1,JMP_CALL_ICON,0,0);
				}
				break;
			case UD_Iret:
			case UD_Iiretw:
			case UD_Iiretd:
			case UD_Iiretq:
					pView->SetHexIcon(0,FUCTION_RET_FLAGS,0,0);
				break;
			}

			//pAsmView->InvalidateRect();
		}

		return TRUE;
	}

	return FALSE;
}

BOOL CZ0BTDisasm::Disassem(
					LPBYTE Start,
					uint64_t DisSize,
					uint64_t Offset,
					CClassViewBase* pView,
					LPTSTR lpSectionName)
{
	if (!pView)
	{
		return FALSE;
	}

	
	CDisasmView* pAsmView = (CDisasmView*)pView->getAsmView();
#ifdef OLD_ITEM_BODY
	int nItem = pAsmView->GetItemCount();
#endif

	ud_set_input_buffer(&ud_obj, Start, DisSize);
	ud_set_pc(&ud_obj,(uint64_t)Offset);

	BOOL bIsX64 = (ud_obj.dis_mode == 64);
	while (ud_disassemble(&ud_obj) != 0)
	{
		tagOption* Option = NULL;
		// printf("\t%s\n", ud_insn_asm(&ud_obj));
		int nSize = 0;
		if (ParseAsm(ud_obj,Option,nSize) && nSize)
		{
			TCHAR szAddress[64] = {0};

// 			if (ud_insn_off(&ud_obj) == 0x11500)
// 			{
// 				int i = 0;
// 				i++;
// 			}
			if (lpSectionName)
			{
				if (bIsX64)
				{
					StringCchPrintf( szAddress,64,_T(" %s:%016llX"),lpSectionName,ud_insn_off(&ud_obj));
				}
				else
				{
					StringCchPrintf( szAddress,64,_T(" %s:%08X"),lpSectionName,ud_insn_off(&ud_obj));
				}
				
			}
			else
			{
				if (bIsX64)
				{
					StringCchPrintf( szAddress,64,_T(" %016llX"),ud_insn_off(&ud_obj));
				}
				else
				{
					StringCchPrintf( szAddress,64,_T(" %08X"),ud_insn_off(&ud_obj));
				}
				
			}
#ifdef OLD_ITEM_BODY		
			pAsmView->InsertItem(nItem,ud_insn_off(&ud_obj),szAddress);
#else
			PCustomItemHead pItem = pAsmView->InsertItem(ud_insn_off(&ud_obj),szAddress);

			if (pItem)
			{
				switch (ud_obj.mnemonic)
				{
				case UD_Ijo:
				case UD_Ijno:
				case UD_Ijb:
				case UD_Ijae:
				case UD_Ijz:
				case UD_Ijnz:
				case UD_Ijbe:
				case UD_Ija:
				case UD_Ijs:
				case UD_Ijns:
				case UD_Ijp:
				case UD_Ijnp:
				case UD_Ijl:
				case UD_Ijge:
				case UD_Ijle:
				case UD_Ijg:
				case UD_Ijcxz:
				case UD_Ijecxz:
				case UD_Ijrcxz:
				case UD_Ijmp:
					{
						pItem->SetInstructionType(kInstructionJmp);
						if ( ud_obj.operand[0]._legacy >= ud_moudle_base &&
							ud_obj.operand[0]._legacy <= (ud_moudle_base + ud_moudle_size) )
						{
							if (ud_relove_skip_jmp)
							{
								ud_relove_skip_jmp( ud_obj, pItem ,ud_obj.operand[0]._legacy,kJmpOther);
							}
						}
					}
					break;
				case UD_Icall:
					{
						pItem->SetInstructionType(kInstructionCall);
						if ( ud_obj.operand[0]._legacy >= ud_moudle_base &&
							ud_obj.operand[0]._legacy <= (ud_moudle_base + ud_moudle_size) )
						{
							if (ud_relove_skip_jmp)
							{
								ud_relove_skip_jmp( ud_obj, pItem ,ud_obj.operand[0]._legacy,kCallFar);
							}
						}
					}
					break;
				case UD_Iret:
				case UD_Iiretw:
				case UD_Iiretd:
				case UD_Iiretq:
					{
						pItem->SetInstructionType(kInstructionRet);

					//	if ( ud_obj.operand[0]._legacy >= ud_moudle_base &&
					//		ud_obj.operand[0]._legacy <= (ud_moudle_base + ud_moudle_size) )
						{
							if (ud_relove_skip_jmp)
							{
								ud_relove_skip_jmp( ud_obj, pItem ,ud_obj.operand[0]._legacy,kRetType);
							}
						}
					}
					break;
				case UD_Imov:
				case UD_Imovapd:
				case UD_Imovaps:
				case UD_Imovd:
				case UD_Imovhpd:
				case UD_Imovhps:
				case UD_Imovlhps:
				case UD_Imovlpd:
				case UD_Imovlps:
				case UD_Imovhlps:
				case UD_Imovmskpd:
				case UD_Imovmskps:
				case UD_Imovntdq:
				case UD_Imovnti:
				case UD_Imovntpd:
				case UD_Imovntps:
				case UD_Imovntq:
				case UD_Imovq:
				case UD_Imovsb:
				case UD_Imovsw:
				case UD_Imovsd:
				case UD_Imovsq:
				case UD_Imovss:
				case UD_Imovsx:
				case UD_Imovupd:
				case UD_Imovups:
				case UD_Imovzx:
					{
						pItem->SetInstructionType(kInstructionMov);
					}
					break;
				case UD_Iint:
					{
						pItem->SetInstructionType(kInstructionInt);
					}
					break;
				case UD_Iint1:
					{
						pItem->SetInstructionType(kInstructionInt1);
					}
					break;
				case UD_Iint3:
					{
						pItem->SetInstructionType(kInstructionInt3);
					}
					break;
				case UD_Iadd:
					{
						if (ud_obj.inp_ctr == 2)
						{
							if ( ud_obj.insn_hexcode[0] == '0' &&
								ud_obj.insn_hexcode[1] == '0' &&
								ud_obj.insn_hexcode[2] == '0' &&
								ud_obj.insn_hexcode[3] == '0' )
							{
								pItem->SetInstructionType(kInstruction0000);
							}
						}
					}
					break;
				case UD_Iinvalid:
					{
						pItem->SetInstructionType(kInstructionInvalid);
					}
					break;
				default:
					break;
				}
			}
			
#endif
			StringCchPrintf( szAddress,64,_T(" %s"),ud_insn_hex(&ud_obj));

#ifdef OLD_ITEM_BODY
			pAsmView->InsertAddressColumn(nItem,4,szAddress);
			pAsmView->SetColumnItem(nItem,2,0,Option,nSize);
#else
			pAsmView->InsertAddressColumn(pItem,4,szAddress);

			if( ud_relove_skip_jmp)
			{
				ud_relove_skip_jmp( ud_obj, pItem ,0,0);
			}

			pAsmView->SetColumnItem(pItem,2,0,Option,nSize);
#endif

			switch (ud_obj.mnemonic)
			{
			case UD_Ijo:
			case UD_Ijno:
			case UD_Ijb:
			case UD_Ijae:
			case UD_Ijz:
			case UD_Ijnz:
			case UD_Ijbe:
			case UD_Ija:
			case UD_Ijs:
			case UD_Ijns:
			case UD_Ijp:
			case UD_Ijnp:
			case UD_Ijl:
			case UD_Ijge:
			case UD_Ijle:
			case UD_Ijg:
			case UD_Ijcxz:
			case UD_Ijecxz:
			case UD_Ijrcxz:
			case UD_Ijmp:
				{
#ifdef OLD_ITEM_BODY
					pAsmView->SetAddressColumnIcon(nItem,3,JMP_OTHER_BODY_FLAGS,0,0);
#else
					pAsmView->SetAddressColumnIcon(pItem,3,JMP_OTHER_BODY_FLAGS,0,0);
#endif
				}
			case UD_Icall:
#ifdef OLD_ITEM_BODY
				pAsmView->SetAddressColumnIcon(nItem,1,JMP_CALL_ICON,0,0);
#else
				pAsmView->SetAddressColumnIcon(pItem,1,JMP_CALL_ICON,0,0);
#endif
				break;
			case UD_Iret:
			case UD_Iiretw:
			case UD_Iiretd:
			case UD_Iiretq:
				pAsmView->SetAddressColumnIcon(pItem,0,FUCTION_RET_FLAGS,0,0);
				
				break;
			}
#ifdef OLD_ITEM_BODY
			nItem++;

			if (nItem > MAX_DISASM_LINES)
			{
				break;
			}
#endif
			pAsmView->InvalidateRect();
		}
		
	}
	//pAsmView->SetScollLines(20000);
#ifdef OLD_ITEM_BODY
	pAsmView->SetScollLines(nItem);
#endif
	pAsmView->InvalidateRect();

	return FALSE;
}

int g_index = 0;

BOOL CZ0BTDisasm::ParseAsm(ud_t& ud,tagOption*& Option,int& nSize)
{
	Option = NULL;
	nSize = 0;
	BOOL bIntStep = FALSE;
	BOOL bIsJmpOther = FALSE;
	const LPTSTR pAsmBuf = ud.asm_buf;
	tagDisBufferInfo* pAsmList = NULL;
	int nCount = 0;

	if (ud.insn_offset == 0x11310)
	{
		int k = 0;
		k++;
	}

	if (ud.mnemonic == UD_Iinvalid)
	{
		nCount = GetAsmBufCount(ud,pAsmList);

		if (nCount && pAsmList )
		{
			pAsmList[0].NorFontColor = pAsmList[0].SelFontColor = DATA_ADDRESS_COLOR;
		}
	}
	else
	{
		nCount = GetAsmBufCount(ud,pAsmList);

		if (nCount && pAsmList )
		{
			// 判断是否是跳转地址
			switch (ud_obj.mnemonic)
			{
			case UD_Ijo:
			case UD_Ijno:
			case UD_Ijb:
			case UD_Ijae:
			case UD_Ijz:
			case UD_Ijnz:
			case UD_Ijbe:
			case UD_Ija:
			case UD_Ijs:
			case UD_Ijns:
			case UD_Ijp:
			case UD_Ijnp:
			case UD_Ijl:
			case UD_Ijge:
			case UD_Ijle:
			case UD_Ijg:
			case UD_Ijcxz:
			case UD_Ijecxz:
			case UD_Ijrcxz:
				{
					pAsmList[0].FontBgColor = JL_BK_INS_COLOR;
					pAsmList[1].FontBgColor = JL_BK_INS_COLOR;
					pAsmList[0].SelFontColor = pAsmList[0].NorFontColor = JL_FONT_INS_COLOR;
					bIsJmpOther = TRUE;
				}
				break;
			case UD_Ipush:
			case UD_Ipusha:
			case UD_Ipushad:
			case UD_Ipushfw:
			case UD_Ipushfd:
			case UD_Ipushfq:
			case UD_Ipop:
			case UD_Ipopa:
			case UD_Ipopad:
			case UD_Ipopfw:
			case UD_Ipopfd:
			case UD_Ipopfq:
				{
					pAsmList[0].SelFontColor = pAsmList[0].NorFontColor = PUSH_POP_FONT_INS_COLOR;
				}
				break;
			case UD_Ijmp:
				{
					pAsmList[0].FontBgColor = JL_BK_INS_COLOR;
					pAsmList[1].FontBgColor = JL_BK_INS_COLOR;

					bIsJmpOther = TRUE;
				}
				break;
			case UD_Icall:
			case UD_Iret:
				{
					pAsmList[0].FontBgColor = RET_CALL_BK_INS_COLOR;

					if (ud.mnemonic == UD_Icall )
					{
						if (ud.operand[0].type != UD_NONE &&
							ud.operand[0].type != UD_OP_REG &&
							ud.operand[0].type != UD_OP_MEM)
						{
							pAsmList[1].FontBgColor = JL_BK_INS_COLOR;
						}
						else if ( ud.operand[0].type == UD_OP_REG ||
							ud.operand[0].type == UD_OP_MEM)
						{
							pAsmList[1].FontBgColor = -1;
							pAsmList[1].NorFontColor = pAsmList[1].SelFontColor = CALL_IMPORT_ADDRESS_COLOR;
						}

					}

				}
				break;
			case UD_Iint3:
			case UD_Iint:
			case UD_Iint1:
				{
					pAsmList[0].NorFontColor = pAsmList[0].SelFontColor = DATA_ADDRESS_COLOR;
				}
				break;
			default:
				{

				}
				break;
			}

			if ( UD_Icall != ud_obj.mnemonic)
			{
				if (ud.operand[0].type == UD_OP_IMM)
				{
					pAsmList[1].NorFontColor = pAsmList[1].SelFontColor = OP_IMM_FONT_COLOR;
				}
				else if (ud.operand[0].type == UD_OP_MEM)
				{
					pAsmList[1].FontBgColor = OP_MEM_BK_COLOR;
				}
			}

			if (ud.operand[1].type == UD_OP_IMM)
			{
				pAsmList[3].NorFontColor = pAsmList[3].SelFontColor = OP_IMM_FONT_COLOR;
			}
			else if (ud.operand[1].type == UD_OP_MEM)
			{
				pAsmList[3].FontBgColor = OP_MEM_BK_COLOR;
			}

			if (ud.operand[2].type == UD_OP_IMM)
			{
				pAsmList[5].NorFontColor = pAsmList[5].SelFontColor = OP_IMM_FONT_COLOR;
			}
			else if (ud.operand[2].type == UD_OP_MEM)
			{
				pAsmList[5].FontBgColor = OP_MEM_BK_COLOR;
			}
		}
	}

	Option = new tagOption[nCount];
	if (!Option)
	{
		goto _Finally;
	}

	for ( int i = 0 ; i < nCount ; i++ )
	{
		Option[i].FontBgColor = pAsmList[i].FontBgColor;				
		Option[i].NorFontColor = pAsmList[i].NorFontColor;
		Option[i].SelFontColor = pAsmList[i].SelFontColor;
		Option[i].lpText = pAsmList[i].lpText;
		Option[i].TextSize = GetTextSize(pAsmList[i].lpText);
		Option[i].HasNonFocus = FALSE;
		pAsmList[i].lpText = NULL;

	}

_Finally:
	delete[] pAsmList;
	pAsmList = NULL;

	nSize = nCount;	


	return TRUE;
}

BOOL CZ0BTDisasm::DisassenCopy(LPTSTR DstBuf,const char* AsmBuf)
{
	DstBuf[0] = 0;
	int Length = strlen(AsmBuf);

	for( int i = 0; i < Length;i++)
	{
		DstBuf[i] = (unsigned short)AsmBuf[i];
	}

	if (DstBuf[0])
	{
		return TRUE;
	}

	return FALSE;
}

int  CZ0BTDisasm::GetAsmBufCount(const char* AsmBuf,LPTSTR*& DstBuf)
{
	//int Length = strlen(AsmBuf);
	LPTSTR* NewTmp = new LPTSTR[MAX_LPTSTR_LIST_SIZE];
	TCHAR TempBuffer[100] = {0};
	int j = 0;
	int k = 0;

	if (NewTmp)
	{
		memset(
			NewTmp,
			0,
			MAX_LPTSTR_LIST_SIZE*sizeof(LPTSTR));

		for( int i = 0; i < 128 ; i++)
		{
			TempBuffer[j] = AsmBuf[i];								

			if (TempBuffer[j] == 0x20 || 
				/*AsmBuf[i] == ',' ||*/
				(AsmBuf[i] == '\0' || (AsmBuf[i] == 0x20 && AsmBuf[i+1] == '\0')))
			{
				while( AsmBuf[i] == 0x20 && AsmBuf[i + 1] == 0x20 && i + 1 < 128 )
				{
					i++;
				}

				TempBuffer[j] = '\0';
				NewTmp[k] = new TCHAR[j + 1];

				if ( !NewTmp[k] )
				{
					goto _ERROR;
				}

				StringCchCopy(
					NewTmp[k],
					j + 1,
					TempBuffer);

// 				if (AsmBuf[i] == ',')
// 				{
// 					NewTmp[++k] = new TCHAR[sizeof(",") + 1];
// 					if ( !NewTmp[k] )
// 					{
// 						goto _ERROR;
// 					}
// 
// 					StringCchCopy(
// 						NewTmp[k],
// 						sizeof(",") + 1,
// 						_T(","));
// 				}

				k++;
				if (AsmBuf[i] == '\0' ||  (AsmBuf[i] == 0x20 && AsmBuf[i+1] == '\0') )
				{
					DstBuf = new LPTSTR[k];
					if (DstBuf)
					{
						memcpy( DstBuf , NewTmp , k* sizeof(LPTSTR));
						return k;
					}
					
				}

				
				j = 0;

				continue;
			}
			
			j++;
		}
	}

_ERROR:
	
	if (NewTmp)
	{
		for( int i = 0; i < MAX_LPTSTR_LIST_SIZE;i++)
		{
			if (NewTmp[i])
			{
				delete[] NewTmp[i];
				NewTmp[i] = NULL;
			}
		}
	}

	return 0;
}

int CZ0BTDisasm::GetAsmBufCount(ud_t& ud,tagDisBufferInfo*& DstBuf)
{
	int Index = 0;
	int nCursor = 0;
	if (ud.operand_asm_buf)
	{
		for( int i = 0; i < 4;i++)
		{
			if (ud.operand_asm_buf[i][0])
			{
				Index++;
			}
			else
			{
				break;
			}

		}

		if (Index <= 0 )
		{
			DstBuf = 0;
			return 0;
		}

		if ( Index > 2 && Index < 4 )
		{
			Index++;
		}
		else if (Index > 3)
		{
			Index += 2;
		}

		DstBuf = new tagDisBufferInfo[Index];
		if (DstBuf)
		{
			memset(
				DstBuf,
				0,
				Index*sizeof(tagDisBufferInfo));

			int MaxIndex = Index < 3?Index:2;

			for( int i = 0; i < MaxIndex;i++)
			{
				int Len = lstrlen(ud.operand_asm_buf[i]);
				if (Len)
				{
					DstBuf[i].lpText = new TCHAR[Len + 1];
					if (DstBuf[i].lpText)
					{
						StringCchCopy(DstBuf[i].lpText,Len + 1,ud.operand_asm_buf[i]);
						DstBuf[i].lpText[Len] = 0;
					}						
				}

				nCursor++;
				DstBuf[i].FontBgColor = -1;
				DstBuf[i].NorFontColor = DstBuf[i].SelFontColor = NORMAL_TEXT_COLOR;
			}

			if (Index > 2)
			{
				DstBuf[nCursor].lpText = new TCHAR[strlen(",") + 1];
				if (DstBuf[nCursor].lpText)
				{
					StringCchCopy(DstBuf[nCursor].lpText,strlen(",") + 1,_T(","));
				}
				DstBuf[nCursor].FontBgColor = -1;
				DstBuf[nCursor].NorFontColor = DstBuf[nCursor].SelFontColor = NORMAL_TEXT_COLOR;
				nCursor++;

				for( int i = 2; i < 3;i++)
				{
					int Len = lstrlen(ud.operand_asm_buf[i]);

					if (Len)
					{
						DstBuf[nCursor].lpText = new TCHAR[Len + 1];
						if (DstBuf[nCursor].lpText)
						{
							StringCchCopy(DstBuf[nCursor].lpText,Len + 1,ud.operand_asm_buf[i]);
							DstBuf[nCursor].lpText[Len] = 0;
						}						
					}

					DstBuf[nCursor].FontBgColor = -1;
					DstBuf[nCursor].NorFontColor = DstBuf[nCursor].SelFontColor = NORMAL_TEXT_COLOR;
					nCursor++;
				}

				if ( Index > 5 )
				{
					DstBuf[nCursor].lpText = new TCHAR[strlen(",") + 1];
					if (DstBuf[nCursor].lpText)
					{
						StringCchCopy(DstBuf[4].lpText,strlen(",") + 1,_T(","));
					}
					DstBuf[nCursor].FontBgColor = -1;
					DstBuf[nCursor].NorFontColor = DstBuf[nCursor].SelFontColor = NORMAL_TEXT_COLOR;
					nCursor++;

					for( int i = 3; i < 4;i++)
					{
						int Len = lstrlen(ud.operand_asm_buf[i]);

						if (Len)
						{
							DstBuf[nCursor].lpText = new TCHAR[Len + 1];
							if (DstBuf[nCursor].lpText)
							{
								//DisassenCopy(DstBuf[i+2].lpText,ud.operand_asm_buf[i]);
								StringCchCopy(DstBuf[nCursor].lpText,Len + 1,ud.operand_asm_buf[i]);
								DstBuf[nCursor].lpText[Len] = 0;
							}							
						}

						DstBuf[nCursor].FontBgColor = -1;
						DstBuf[nCursor].NorFontColor = DstBuf[nCursor].SelFontColor = NORMAL_TEXT_COLOR;
						nCursor++;
					}
				}
			}

			return Index;

		}
	}


__ERROR:
	if (DstBuf)
	{
		delete[] DstBuf;
		DstBuf = NULL;
	}

	return 0;
}



CViewData::CViewData()
{

}

CViewData::~CViewData()
{

}


LPVOID CViewData::Insert(uint64_t uOffset,LPTSTR szText)
{
	return m_ViewHead.Insert(uOffset,szText);
}

LPVOID CViewHead::Insert(uint64_t uOffset,LPTSTR szText)
{
	if (szText)
	{
		int Length = lstrlen(szText);
		m_szText				= new TCHAR[Length + 1];
		if (m_szText)
		{
			m_NorFontColor			= DATA_ADDRESS_COLOR;
			m_NoneFocusFontColor	= DATA_ADDRESS_COLOR;
			m_SelFontColor			= DATA_ADDRESS_SELECT_COLOR;
			m_SelBgkBrush			= 0;
			m_NorBgkBrush			= 0;
			m_u64Address			= uOffset;

			StringCchCopy(
				m_szText,
				Length + 1,
				szText
				);

			return m_szText;
		}
	}

	return 0;
}

void CViewData::SetInstructionType( uint8_t InsType)
{
	m_ViewHead.SetInstructionType(InsType);
}

void CViewHead::SetInstructionType( uint8_t InsType)
{
	m_uInstrType = InsType;
}

void CViewData::InsertHex( int IconNums,LPTSTR lpText )
{
	m_ViewColumn[0].InsertHex(IconNums,lpText);
}

void CViewColumn::InsertHex( int IconNums,LPTSTR lpText )
{
	m_Icon = new tagIcon[IconNums];
	if (m_Icon)
	{
		memset(m_Icon,0,IconNums*sizeof(tagIcon));
		m_IconCount = IconNums;
		for (int i = 0;i < IconNums ;i++)
		{
			m_Icon[i].nIcon = 0;
			m_Icon[i].colorIcon = NORMAL_TEXT_COLOR;

		}
	}

	m_Option = new tagOption[1];
	if (m_Option)
	{
		memset(m_Option,0,sizeof(tagOption));
		m_OptionCount = 1;
		m_Option->NorFontColor = DATA_ADDRESS_SELECT_COLOR;
		m_Option->SelFontColor = DATA_ADDRESS_SELECT_COLOR;
		m_Option->HasNonFocus  = DATA_ADDRESS_COLOR;
		m_Option->FontBgColor  = -1;

		if (lpText)
		{
			int Length = lstrlen(lpText);

			if (Length)
			{
				m_Option->TextSize = Length;
				m_Option->lpText = new TCHAR[Length + 1];
				if (m_Option->lpText)
				{
					StringCchCopy( 
						m_Option->lpText,
						Length + 1,
						lpText);
				}
			}
		}
	}	
}

void CViewData::SetHexIcon(int IconNums,int nIconType,int nIconType2,int nIconType3 )
{
	m_ViewColumn[0].SetHexIcon(IconNums,nIconType,nIconType2,nIconType3);
}

void CViewColumn::SetHexIcon(int IconNums,int nIconType,int nIconType2,int nIconType3 )
{
	if (IconNums < m_IconCount )
	{
		// 			for ( int i = 0;
		// 				i < m_IconCount ;
		// 				i++)
		{
			m_Icon[IconNums].nIcon = nIconType;
			m_Icon[IconNums].colorIcon = nIconType2;
		}
	}
}

void    CViewData::SetViewColumn(
				int nColumn,
				int IconNums,
				tagOption* Option,
				int nSize )
{
	if ( nColumn >= 0 &&
		nColumn < CUSTOM_MAX_COLUMN)
	{
		m_ViewColumn[nColumn].SetViewColumn(IconNums,Option,nSize);
	}
}

void    CViewColumn::SetViewColumn(
						int IconNums,
						tagOption* Option,
						int nSize )
{
	//ColumnData[nColumn-1].SelBgkBrush = hBgkSelect;
	//ColumnData[nColumn-1].NorBgkBrush = hBgkBrush;

	if (IconNums)
	{
		m_Icon = new tagIcon[IconNums];
		if (m_Icon)
		{
			memset(m_Icon,0,IconNums*sizeof(tagIcon));
			m_IconCount = IconNums;
			for (int i = 0;i < IconNums ;i++)
			{
				m_Icon[i].nIcon = 0;
				m_Icon[i].colorIcon = NORMAL_TEXT_COLOR;

			}
		}
	}			


	if (nSize)
	{
		m_Option = Option;
		m_OptionCount = nSize;
	}
}


tagIcon* CViewData::GetColumnIcon( int nColumn )
{
	if (nColumn >= 0 && nColumn < CUSTOM_MAX_COLUMN)
	{
		return m_ViewColumn[nColumn].GetIcon();
	}

	return NULL;	
}

int	CViewData::GetColumnIconCount(int nColumn)
{
	if (nColumn >= 0 && nColumn < CUSTOM_MAX_COLUMN)
	{
		return m_ViewColumn[nColumn].GetIconCount();
	}

	return 0;
}

tagOption* CViewData::GetColumnOption( int nColumn )
{
	if (nColumn >= 0 && nColumn < CUSTOM_MAX_COLUMN)
	{
		return m_ViewColumn[nColumn].GetOption();
	}

	return NULL;
}

int	CViewData::GetColumnOptionCount(int nColumn)
{
	if (nColumn >= 0 && nColumn < CUSTOM_MAX_COLUMN)
	{
		return m_ViewColumn[nColumn].GetOptionCount();
	}

	return NULL;
}