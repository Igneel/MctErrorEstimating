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
	TLabeledEdit *eKFactor;
	TLabeledEdit *eHeavyHoleConcentration;
	TLabeledEdit *eAFactor;
	TLabeledEdit *eSampleThickness;
	TLabeledEdit *eCurrentIntensity;
	TLabeledEdit *eTemperature;
	TLabeledEdit *eMolarCompositionCadmium;
	TLabeledEdit *eCBRatio;
	TButton *bWhiteNoiseGenerator;
	TButton *bGaussianNoiseGenerator;
	TButton *bCalculateCarrierParams;
	TMemo *mDebug;
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
	TButton *bSaveAllPoints;
	TEdit *Edit5;
	TLabel *Label8;
	TLabel *Label9;
	TLabel *Label10;
	TEdit *Edit6;
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
	TButton *tenzorCalculating;
	TButton *bAutomaticCalculation;
	TEdit *eStepByTemperature;
	TLabel *Label11;
	TButton *bFilteringPlots;
	TLineSeries *Series7;
	TLineSeries *Series8;
	TButton *bSaveElevenPoints;
	TGroupBox *gbSavingPlot;
	TRadioButton *rbLeftPlot;
	TRadioButton *rbRightPlot;
	TRadioButton *rbFilteredPlot;
	TButton *bLoadingPlots;
	TEdit *eFilterLength;
	TRadioButton *rbIdealPlot;
	TButton *bSaveFilmParams;
	TLabel *Label12;
	TButton *bTestingSomething;
	TLineSeries *Series9;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall bCalculateCarrierParamsClick(TObject *Sender);
	void __fastcall bGaussianNoiseGeneratorClick(TObject *Sender);
	void __fastcall bSaveAllPointsClick(TObject *Sender);
	void __fastcall g_Nz_parKeyPress(TObject *Sender, wchar_t &Key);
	void __fastcall bWhiteNoiseGeneratorClick(TObject *Sender);
	void __fastcall Button7Click(TObject *Sender);
	void __fastcall tenzorCalculatingClick(TObject *Sender);
	void __fastcall bAutomaticCalculationClick(TObject *Sender);
	void __fastcall bFilteringPlotsClick(TObject *Sender);
	void __fastcall bSaveElevenPointsClick(TObject *Sender);
	void __fastcall bLoadingPlotsClick(TObject *Sender);
	void __fastcall Button13Click(TObject *Sender);
	void __fastcall bSaveFilmParamsClick(TObject *Sender);
	void __fastcall bTestingSomethingClick(TObject *Sender);





private:	// User declarations
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
long double niSob(long double T, long double x);
//---------------------------------------------------------------------------
#endif
