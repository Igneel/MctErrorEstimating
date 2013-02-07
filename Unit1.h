//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "Chart.hpp"
#include "Series.hpp"
#include "TeEngine.hpp"
#include "TeeProcs.hpp"
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Dialogs.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TPageControl *PG;
	TTabSheet *TabSheet1;
	TTabSheet *Графики;
	TLabeledEdit *LabeledEdit7;
	TLabeledEdit *LabeledEdit8;
	TLabeledEdit *LabeledEdit6;
	TLabeledEdit *LabeledEdit1;
	TLabeledEdit *LabeledEdit3;
	TLabeledEdit *LabeledEdit5;
	TLabeledEdit *LabeledEdit4;
	TLabeledEdit *LabeledEdit2;
	TButton *Button5;
	TButton *Button3;
	TButton *Button1;
	TMemo *Memo1;
	TButton *Button6;
	TButton *Button7;
	TStringGrid *g_Nz_par;
	TEdit *g_hsize;
	TLabel *Label1;
	TChart *Chart1;
	TLineSeries *Series1;
	TChart *Chart2;
	TLineSeries *Series2;
	TChart *Chart4;
	TLineSeries *Series4;
	TChart *Chart3;
	TLineSeries *Series3;
	TButton *Button4;
	TEdit *Edit5;
	TLabel *Label8;
	TLabel *Label9;
	TLabel *Label10;
	TEdit *Edit6;
	TButton *Button2;
	TEdit *Edit1;
	TLabel *Label2;
	TLabel *Label4;
	TLabel *Label7;
	TEdit *Edit2;
	TLabel *Label3;
	TLabel *Label5;
	TLabel *Label6;
	TEdit *Edit4;
	TEdit *Edit3;
	TChart *Chart5;
	TLineSeries *Series5;
	TLineSeries *Series6;
	TChart *Chart6;
	TLineSeries *LineSeries1;
	TLineSeries *LineSeries2;
	TChart *Chart7;
	TLineSeries *LineSeries3;
	TLineSeries *LineSeries4;
	TChart *Chart8;
	TLineSeries *LineSeries5;
	TLineSeries *LineSeries6;
	TChart *Chart9;
	TLineSeries *LineSeries7;
	TLineSeries *LineSeries8;
	TChart *Chart10;
	TLineSeries *LineSeries9;
	TLineSeries *LineSeries10;
	TSaveDialog *sg1;
	TButton *Button8;
	TButton *Button9;
	TEdit *Edit7;
	TLabel *Label11;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall Button4Click(TObject *Sender);
	void __fastcall g_Nz_parKeyPress(TObject *Sender, wchar_t &Key);
	void __fastcall Button5Click(TObject *Sender);
	void __fastcall Button6Click(TObject *Sender);
	void __fastcall Button7Click(TObject *Sender);
	void __fastcall Button8Click(TObject *Sender);
	void __fastcall Button9Click(TObject *Sender);

private:	// User declarations
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
long double niSob(long double T, long double x);
//---------------------------------------------------------------------------
#endif
