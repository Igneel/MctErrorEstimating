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
	TTabSheet *�������;
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
	TButton *BuildingPlots;
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
	TLineSeries *gSeriesIdealParamsSxx;
	TLineSeries *gSeriesParamsWithNoiseSxx;
	TChart *Chart6;
	TLineSeries *gSeriesIdealParamsS_eff;
	TLineSeries *gSeriesParamsWithNoiseS_eff;
	TChart *Chart7;
	TLineSeries *gSeriesIdealParamsUs;
	TLineSeries *gSeriesParamsWithNoiseUs;
	TChart *Chart8;
	TLineSeries *gSeriesIdealParamsSxy;
	TLineSeries *gSeriesParamsWithNoiseSxy;
	TChart *Chart9;
	TLineSeries *gSeriesIdealParamsRh_eff;
	TLineSeries *gSeriesParamsWithNoiseRh_eff;
	TChart *Chart10;
	TLineSeries *gSeriesIdealParamsUy;
	TLineSeries *gSeriesParamsWithNoiseUy;
	TSaveDialog *sg1;
	TButton *bAutomaticCalculation;
	TEdit *eStepByTemperature;
	TLabel *Label11;
	TButton *bFilteringPlots;
	TLineSeries *gSeriesFilteredUs;
	TLineSeries *gSeriesFilteredUy;
	TButton *bSaveElevenPoints;
	TGroupBox *gbSavingPlot;
	TRadioButton *rbFilteredUPlot;
	TButton *bLoadingPlots;
	TEdit *eFilterLength;
	TRadioButton *rbIdealUPlot;
	TButton *bSaveFilmParams;
	TLabel *Label12;
	TButton *bTestingSomething;
	TLineSeries *Series9;
	TLineSeries *gSeriesExtrapolatedUs;
	TLineSeries *gSeriesExtrapolatedUy;
	TLineSeries *gSeriesFilteredParamsSxx;
	TLineSeries *gSeriesExtrapolatedParamsSxx;
	TLineSeries *gSeriesFilteredParamsSxy;
	TLineSeries *gSeriesExtrapolatedParamsSxy;
	TLineSeries *gSeriesExtrapolatedParamsS_eff;
	TLineSeries *gSeriesFilteredParamsS_eff;
	TLineSeries *gSeriesExtrapolatedParamsRh_eff;
	TLineSeries *gSeriesExtrapolatedParamsUs;
	TLineSeries *gSeriesExtrapolatedParamsUy;
	TLineSeries *gSeriesFilteredParamsRh_eff;
	TLineSeries *gSeriesFilteredParamsUs;
	TLineSeries *gSeriesFilteredParamsUy;
	TRadioButton *rbIdealTenzorPlot;
	TRadioButton *rbFilteredTenzor;
	TRadioButton *rbExtrapolatedU;
	TRadioButton *rbExtrapolatedTenzor;
	TRadioButton *rbNoisyU;
	TRadioButton *rbNoisyTenzor;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall bCalculateCarrierParamsClick(TObject *Sender);
	void __fastcall bGaussianNoiseGeneratorClick(TObject *Sender);
	void __fastcall bSaveAllPointsClick(TObject *Sender);
	void __fastcall g_Nz_parKeyPress(TObject *Sender, wchar_t &Key);
	void __fastcall bWhiteNoiseGeneratorClick(TObject *Sender);
	void __fastcall BuildingPlotsClick(TObject *Sender);
	void __fastcall bAutomaticCalculationClick(TObject *Sender);
	void __fastcall bFilteringPlotsClick(TObject *Sender);
	void __fastcall bSaveElevenPointsClick(TObject *Sender);
	void __fastcall bLoadingPlotsClick(TObject *Sender);
	void __fastcall Button13Click(TObject *Sender);
	void __fastcall bSaveFilmParamsClick(TObject *Sender);
	void __fastcall bTestingSomethingClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);




private:	// User declarations
public:		// User declarations
	void automaticCalculationHelper(UnicodeString SaveFileName);
    void RoundM(long double * x,int length);

	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
long double ownConcentrationOfElectrons(long double T, long double x);
void SavingAllPoints(TLineSeries* Series7,TLineSeries* Series8);
void ParamsKRT(void);
//---------------------------------------------------------------------------
#endif
