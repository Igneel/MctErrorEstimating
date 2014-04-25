procedure GetCoef(var A,X:Data_spektr; b:extended;var P0,P1,P2:extended);
var i,j:word;
     r,r1,r2,q,s:extended;
 begin
    p0:=0;p1:=0;p2:=0;
    for i:=0 to NumberOfPoints do
     begin
      r:=1;r1:=0;r2:=0;
      for j:=0 to NumberOfPoints do
       if i<>j then begin
        q:=X[i]-X[j];
        s:=b-X[j];r2:=(r2*s+2*r1)/q;
        r1:=(r1*s+r)/q; r:=r*s/q;
       end;
      p0:=p0+r*A[i];p1:=p1+r1*A[i];p2:=p2+r2*A[i];
     end;
 end;

procedure TForm1.MakeInterpolate;
var temp_l,temp_t,AGxx,AGxy,AField:Data_spektr;
    sf,lm,p,p1:extended;
    i,j:word;
    Lmin,Lmax,k:integer;
	procedure CS(var X,F,C:Data_spektr;p1,pn:extended);
	var i,j,m:word;
	    K:Data_spektr;
	    A,B,R:extended;
	begin
	   K[1]:=0;C[1]:=P1;
	   A:=X[1]-X[0];
	   B:=X[2]-X[1];
	   k[2]:=(3*((F[2]-F[1])/(X[2]-X[1])-(F[1]-F[0])/(X[1]-X[0]))-
	       (X[1]-X[0])*C[1])/2/(A+B);
	   C[2]:=B/2/(A+B);
	   for i:=3 to NumberOfPoints do
	    begin
	     j:=i-1;m:=j-1;
	     A:=X[i]-X[j];B:=X[j]-X[m];R:=2*(A+B)-B*C[j];C[i]:=A/R;
	     K[i]:=(3*((F[i]-F[j])/A-(F[j]-F[m])/B)-B*K[j])/R;
	    end;
	   C[NumberOfPoints]:=K[NumberofPoints]-C[NumberofPoints]*pn;
	   for i:=NumberOfPoints-1 downto 2 do C[i]:=K[i]-C[i]*C[i+1];
	end;
	function Sp(var X,F,C:Data_spektr;x1:extended):extended;
	var i,j:word;
	    A,B,D,Q,R,P:extended;
	begin i:=1; while (X1>X[i]) do inc(i);
	 j:=i-1;A:=F[j];B:=X[j];Q:=X[i]-B;
	 R:=X1-B;P:=C[i];D:=C[i+1];
	 B:=(F[i]-A)/q-(D+2*P)*Q/3;D:=(D-P)/3/Q;
	 SP:=A+R*(B+R*(P+D*R));
	end;
begin
  //Формируем новые матрицы для расчета производных в точке В=0
  AField[0]:=-MagField_spektr[1];
  AGxx[0]:=GxxExp[1];
  AGxy[0]:=-GxyExp[1];
  for i:=1 to NumberOfPoints do
   begin
    AField[i]:=MagField_spektr[i-1];
    AGxx[i]:=GxxExp[i-1];
    AGxy[i]:=GxyExp[i-1];
   end;

   //Вычисляем производные в точке В=0
   dec(NumberOfPoints);
   GetCoef(AGxx,AField,AField[1],p,p1,A1);
   p1:=0;
   GetCoef(AGxy,AField,AField[1],p,p1,B1);
   B1:=0;

   //Вычисляем производные в точке В=Вмах
   GetCoef(GxxExp,MagField_spektr,MagField_spektr[NumberOfPoints],p,p1,An);
   GetCoef(GxyExp,MagField_spektr,MagField_spektr[NumberOfPoints],p,p1,Bn);
   cs(MagField_spektr,GxxExp,temp_l,A1,An);
   cs(MagField_spektr,GxyExp,temp_t,B1,Bn);

   AddExpPoints;
   With Chart5 do
    begin
     Series[0].Clear;
    end;
   with Chart6 do
    begin
     Series[0].Clear;
    end;
   GetLnLimits(Lmin,Lmax);
   SizeData:=(Lmax-Lmin+1)*sizeof(extended)*PointPerInt;
   InitArray;
   k:=0;
   for i:=0 to (lmax-lmin) do
    begin
     lm:=exp((lmin+i)*ln(10));
     sf:=lm;
     for j:=1 to PointPerInt-1 do
      begin
       IntMagField^[k]:=sf;
       IntGxx^[k]:=sp(MagField_spektr,GxxExp,temp_l,sf);
       IntGxy^[k]:=sp(MagField_spektr,GxyExp,temp_t,sf);
       Chart5.Series[0].AddXY(IntMagField^[k],IntGxx^[k],'',clTeeColor);
       Chart6.Series[0].AddXY(IntMagField^[k],IntGxy^[k],'',clTeeColor);
       sf:=lm*exp(j/PointPerInt*ln(10));
       if sf>MagField_spektr[NumberofPoints] then break;
       inc(k);
      end;
     if sf>MagField_spektr[NumberofPoints] then break;
    end;
   AddPoints(Chart5);
   AddPoints(Chart6);
end;

procedure TForm1.MakeMNK( a:boolean);
var tmp_m:mat;
    coef_t,coef_l:Data_spektr;
    kind,i,j,k,Lmin,Lmax:integer;
    lm,sf:extended;
 function pow(x:extended;y:word):extended;
 var p:extended;
     i:word;
  begin
   p:=1;
   for i:=1 to y do p:=p*x;
   pow:=p;
  end;
	procedure BAS(N,M,L:word;X1:extended;var X,T:Data_spektr);
	var
	  k:word;z,r:extended;
	begin
	 z:=2*(x1-x[0])/(x[n]-x[0])-1;
	 t[0]:=1;
	 t[1]:=z;
	 for k:=1 to m-1 do
	 begin
	 r:=z*t[k];
	  case L of
	    1: r:=r-t[k-1]/4;
	    2: r:=2*r-t[k-1];
	    3: r:=((k+k+1)*r-k*t[k-1])/(k+1);
	  end;
	  t[k+1]:=r;
	 end;
	end;
	procedure gram(N,M,L:word;var x,f:Data_spektr; var a:mat);
	var i,j,k:integer;
	    q,r,s:extended;
	    t:Data_spektr;
	    p:array [0..MaxPoints,0..5*MaxPoints] of extended;
	begin
	 for i:=0 to N do begin bas(n,m,l,x[i],x,t);
	  for j:=0 to m do p[j,i]:=t[j]
	 end;
	 for k:=0 to m do begin
	  for j:=k to m do begin s:=0; r:=0;
	   for i:=0 to n do begin q:=p[k,i];s:=s+q*p[j,i];
	    if j=m then r:=r+q*f[i];
	   end; a[k,j]:=s;a[j,k]:=s;
	  end; a[k,m+1]:=r;
	 end;
	end;

	procedure gauss(N:word;var a:mat; var x:Data_spektr);
	 var i,j,k,k1,n1:word; r,s:extended;
	 begin
	   n1:=N+1;
	   for k:=0 to n do begin k1:=k+1;s:=a[k,k];
	    for j:=k1 to n1 do a[k,j]:=a[k,j]/s;
	      for i:=k1 to n do begin r:=a[i,k];
	          for j:=k1 to n1 do a[i,j]:=a[i,j]-a[k,j]*r
	      end;
	    end;
	     for i:=n downto 0 do begin s:=a[i,n1];
	     for j:=i+1 to n do s:=s-a[i,j]*x[j];
	     x[i]:=s;
	    end;
	  end;

	procedure fi(n,m,l:word;var c,x:Data_spektr;var x1,s:extended);
	var i:word;t:Data_spektr;
	begin
	 s:=c[0];
	 bas(n,m,l,x1,x,t);
	 for i:=1 to m do s:=s+c[i]*t[i]
	end;

begin

   Power_spektr:=3;  
   Kind:=2; 
   gram(NumberOfPoints,Power_spektr,Kind,MagField_spektr,GxxExp,tmp_m);
   gauss(Power_spektr,tmp_m,coef_l);
   gram(NumberOfPoints,Power_spektr,Kind,MagField_spektr,GxyExp,tmp_m);
   gauss(Power_spektr,tmp_m,coef_t);
   With Chart5 do
    begin
     Series[0].Clear; // чистим графики компонент
    end;
   with Chart6 do
    begin
     Series[0].Clear;
    end;
   if a then 
     begin
      AddExpPoints; // добавляет точки на график, не логарифмически
      GetLnLimits(Lmin,Lmax); // получает пределы, логарифмические
      SizeData:=(Lmax-Lmin+1)*sizeof(extended)*PointPerInt; // считаем размер данных
      InitArray;  // выделяем его
      k:=0;
      for i:=0 to (lmax-lmin) do
       begin
       lm:=exp((lmin+i)*ln(10));
       sf:=lm;
       for j:=1 to PointPerInt-1 do
        begin
        IntMagField^[k]:=sf;

        fi(NumberOfPoints,Power_spektr,Kind,coef_l,MagField_spektr,sf,IntGxx^[k]);
        fi(NumberOfPoints,Power_spektr,Kind,coef_t,MagField_spektr,sf,IntGxy^[k]);
        Chart5.Series[0].AddXY(IntMagField^[k],IntGxx^[k],'',clTeeColor);
        Chart6.Series[0].AddXY(IntMagField^[k],IntGxy^[k],'',clTeeColor);
        sf:=lm*exp(j/PointPerInt*ln(10));
        if sf>MagField_spektr[NumberofPoints] then break;
        inc(k);
       end;
      if sf>MagField_spektr[NumberofPoints] then break;
     end;
     AddPoints(Chart5);
     AddPoints(Chart6);
    end else
    begin
     for i:=0 to NumberOfPoints do
      begin
        fi(NumberOfPoints,Power_spektr,Kind,coef_l,MagField_spektr,sf,GxxExp[i]);
        fi(NumberOfPoints,Power_spektr,Kind,coef_t,MagField_spektr,sf,GxyExp[i]);
      end;
    end;

end;

procedure TForm1.MakeLagranj;
var X,Y,Y1,Y2:extended;
begin
  X:=MagField_spektr[NumberOfPoints]-(MagField_spektr[NumberOfPoints]-
        MagField_spektr[NumberOfPoints-1])/4;
  GetCoef(GxxExp,MagField_spektr,X,Y,Y1,Y2);
  GxxExp[NumberOfPoints+1]:=GxxExp[NumberOfPoints];
  GxxExp[NumberOfPoints]:=Y;
  GetCoef(GxyExp,MagField_spektr,X,Y,Y1,Y2);
  GxyExp[NumberOfPoints+1]:=GxyExp[NumberOfPoints];
  GxyExp[NumberOfPoints]:=Y;
  MagField_spektr[NumberOfPoints+1]:=MagField_spektr[NumberOfPoints];
  MagField_spektr[NumberOfPoints]:=X;
  inc(NumberOfPoints);
end;

{ J.H.Wilkinson, C.Reinch. Handbook for Automatic Computation.
  Linear Algebra. Springer Verlag: Heidelberg, NewYork, Berlin.
  algorithm II-4.
  
  Procedure Imtql2 calculate all eigenvalues and eigenvectors
  of real tridiagonal symmetric matrix by QL-algorithm
  with implicit shift.
  Unit matrix or matrix of reduction to tridiagonal form by Tred2.}

function GetElem(j1,k1,i1:word):extended;
 var
  s:extended;
  ii:word;
 begin
    s:=0;
    for  ii:=i1 to (NumberOfPoints-(j1-2)) do
     if j1=2 then begin
       if ii<>k1 then s:=s+sqr(B_spektr[ii]) end else
        if ii<>k1 then s:=s+sqr(B_spektr[ii])*GetElem(j1-1,k1,ii+1);
    GetElem:=s;
 end;

procedure MakeMatrC;
var
 j,k:word;
begin
 fillchar(Cr, Sizeof(Cr), 0);
 fillchar(Cl, Sizeof(Cl), 0);
 fillchar(Cr_t,Sizeof(Cr_t),0);
 fillchar(Cl_t,Sizeof(Cl_t),0);
 fillchar(Am, Sizeof(Am), 0);
 for j:=1 to NumberOfPoints do
  for k:=1 to NumberOfPoints do begin
   if j=1 then Cr[j,k]:=1 else
     Cr[j,k]:=GetElem(j,k,1);
     Cl[j,k]:=-Cr[j,k]*B_spektr[k];
    end;
end;

procedure MakeMatrA;
 var i,j,k:word;
 begin
  fillchar(Am,sizeof(Am),0);
  for i:=1 to NumberOfPoints do
   for j:=1 to NumberOfPoints do
    if odd(i+j) then
      for k:=1 to NumberOfPoints do
           Am[i,j]:=Am[i,j]+Gxy_sp[k]*Cl_t[k,(i+j-1) shr 1]
               else
       for k:=1 to NumberOfPoints do
           Am[i,j]:=Am[i,j]+Gxx_sp[k]*Cr_t[k,(i+j) shr 1]
 end;

procedure InverseMatrC(var Ci:dat2;var C:dat2;var Su:extended;NP:word);
  var i,j,k:word;
       at:dat3;
       sr:extended;
  begin
  for i:=1 to Np do
   for j:=1 to NP do
      at[i,j]:=Ci[i,j];
   for i:=1 to NP do begin
    for j:=NP+1 to 2*NP do At[i,j]:=0; At[i,i+NP]:=1;
   end;
   for k:=1 to NP do begin Su:=At[k,k]; j:=k;
    for i:=k+1 to NP  do begin Sr:=At[i,k];
     if abs(Sr)>abs(Su) then begin su:=sr; j:=i; end;
    end;
    if su=0 then exit;
    if j<>k then for i:=k to 2*NP do begin
     sr:=at[k,i];at[k,i]:=at[j,i];at[j,i]:=sr;end;
    for j:=k+1 to 2*NP do at[k,j]:=at[k,j]/su;
    for i:=k+1 to NP do begin sr:=at[i,k];
    for j:=k+1 to 2*NP do at[i,j]:=at[i,j]-at[k,j]*sr
   end;
  end;
  if su<>0 then
   for j:=NP+1 to 2*NP do
    for i:=NP-1 downto 1 do begin sr:=at[i,j];
    for k:=i+1 to NP do sr:=sr-at[k,j]*at[i,k];
    at[i,j]:=sr;
   end;
   if su<>0 then
    for i:=1 to Np do
     for j:=1 to NP do
      C[i,j]:=at[i,j+NP];
  end;

function S_s(Mi:extended):extended;
   var im,js:word;
       a,bb:extended;
   begin
     { make Vm }
     Mv[1]:=1;
     if abs(Mi)<1e-26 then
      for im:=2 to NumberOfPoints do Mv[im]:=0 else
     for im:=2 to NumberOfPoints do
      if odd(im) then
       Mv[im]:=exp((im-1)*ln(abs(Mi))) else
       if Mi<0 then Mv[im]:=exp((im-1)*ln(abs(Mi))) else
          Mv[im]:=-exp((im-1)*ln(abs(Mi)));
     a:=1;
     { compute am/Go }
     for im:=1 to NumberOfPoints do a:=a*(1+sqr(B_spektr[im]*Mi));
     for im:=1 to NumberOfPoints do begin Vpr[im]:=0;
     for js:=1 to NumberOfPoints do
          Vpr[im]:=Vpr[im]+Qm[js,im]*Mv[js];
      Vpr[im]:=sqr(Vpr[im]);end;
     bb:=0;
     for im:=1 to NumberOfPoints do begin
{      if (abs(Lv[im])<1e-30)and(Vpr[im]<1e-15) then continue}
       if (abs(Lv[im])=0)and(Vpr[im]>1e-15) then begin a:=0;break;end;
      bb:=bb+Vpr[im]/abs(Lv[im]); end;
     S_s:=(a/bb);
   end;

procedure TForm1.MobilitySpectrum;
var
  Sf,lm:extended;
  j,k,i:word;
  Lmin,Lmax:integer;
begin
   for i:=0 to NumberOfPoints do 
    begin
     B_spektr[i+1]:=MagField_spektr[i];
     Gxx_sp[i+1]:=GxxExp[i];
     Gxy_sp[i+1]:=GxyExp[i];
    end;
   inc(NumberOfPoints);
   MakeMatrC;
   InverseMatrC(Cr,Cr_t,Sf,NumberOfPoints); 
   
   if Sf=0 then begin MessageDLG('Определитель равен нулю!',mtError,[mbOK],0);
         dec(NumberOfPoints);
         exit; end;
   if MagField_spektr[0]=0 then begin
    for j:=1 to NumberOfPoints-1 do
    for k:=1 to NumberOfPoints-1 do
     Cm[j,k]:=Cl[j,k+1];
     InverseMatrC(Cm,Cm_t,Sf,NumberOfPoints-1);
      for j:=1 to NumberOfPoints do
       for k:=1 to NumberOfPoints do
        if j=1 then Cl_t[1,k]:=0 else
         if k=NumberOfPoints then Cl_t[j,NumberOfPoints]:=0 else
          Cl_t[j,k]:=Cm_t[j-1,k];
    end else
    InverseMatrC(Cl,Cl_t,Sf,NumberOfPoints);
    MakeMatrA;
    Tred2(NumberOfPoints,6e-4913,Lv,Xv,am,Qm,bulua);
    Imtql2(NumberOfPoints,5.42e-20,Lv,Xv,Qm,bulua);

   with Chart3 do // чистим графики
    begin
     Series[0].Clear;
     Series[1].Clear;
    end;
   Lmin:=MSLeft;LMax:=MSRight; 
   SizeData:=(Lmax-Lmin+1)*sizeof(extended)*PointPerInt;
   InitArray2;
   k:=0;
   for i:=0 to (lmax-lmin) do
    begin
     lm:=exp((lmin+i)*ln(10));
     sf:=lm;
     for j:=1 to PointPerInt-1 do // строим графики
      begin
       Mobility^[k]:=sf;
       Spectr_e^[k]:=S_s(-sf);
       Spectr_p^[k]:=S_s(sf);
       Chart3.Series[0].AddXY(Mobility^[k],Spectr_e^[k],'',clTeeColor);
       Chart3.Series[1].AddXY(Mobility^[k],Spectr_p^[k],'',clTeeColor);
       sf:=lm*exp(j/PointPerInt*ln(10));
       inc(k);
       if sf>10 then break;
      end;
     if sf>10 then break;
    end;
    GridPoints:=k-1;
   AddPoints2;
   dec(NumberOfPoints);
end;

function GxxExpi:extended;
var i:word;
begin
  Result:=0;
  for i:=0 to gridPoints do
   result:=result+Axx^[i]*(QSpectr_p^[i]+QSpectr_e^[i]);
end;



      // Загрузка данных из файла в спектр подвижности.
procedure TForm1.Button7Click(Sender: TObject);

   // в этих двух функция - всё что нужно, чтобы посчитать спектр подвижности.
   MakeMNK(true); // судя по названию - метод наименьших квадратов
   // а судя по смыслу - должен быть метод наименьших квадратов.
   MobilitySpectrum;  // спектр подвижности. Начало.


////////////////////////////////////////////////////////////////////////////
/////////////////////// КОНЕЦ "ХОЛЛ. ПОДВИЖНОСТЬ"///////////////////////////
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
///////////////// НАЧАЛО "ХОЛЛ. МНОГОЗОННАЯ ПОДГОНКА" //////////////////////
////////////////////////////////////////////////////////////////////////////

function func_hall8:Extended;
var   myi                     :integer;
     g,g1,cond1,cond2,cond3:Extended;
begin
for myi:=0 to NPoint-1 do
  begin
  cond1:=Data[4]*Data[1]/(1+sqr(data[1]*Magfield_spektr[myi]));
  cond2:=Data[5]*Data[2]/(1+sqr(data[2]*MagField_spektr[myi]));
  cond3:=Data[6]*Data[3]/(1+sqr(data[3]*MagField_spektr[myi]));
  Gxx[myi]:=e*(cond1+cond2+cond3);
  if myi=0 then
   Gxy[0]:=0
  else
   Gxy[myi]:=(cond1*data[1]+cond2*data[2]+cond3*data[3])
        *MagField_spektr[myi]*e;
  end;
   g:=0;g1:=0;
  for myi:=0 to NPoint-1 do
  begin
     g:=g+sqr((Gxx[myi]-GxxExp[myi])/(GxxExp[myi]+Gxx[myi]));
  
   if myi=0 then g1:=0 else
    begin
     g1:=g1+sqr((Gxy[myi]-GxyExp[myi])/(abs(GxyExp[myi])+abs(Gxy[myi])));
    end;
  end;
   
  func_hall8:=100*(sqrt(g*Ves1)+sqrt(g1*Ves2))/NPoint;
end;

procedure Optimiz_hall8;
begin
  if (Form1.Edit7.Text<>'') and (Form1.Edit8.Text<>'') then
    begin
     Ves1:=StrToFloat(Form1.Edit7.Text);
     Ves2:=StrToFloat(Form1.Edit8.Text);
    end
   else
    begin
      Ves1:=1;
      Ves2:=1;
    end;
  Randomize;
  N_Data:=6;
  Registerfunc(func_hall8);
  Registergraph(Graphik_hall8);
  BegRand(1000);   // в данном случае c 1000 гараздо лучше ищет
  hook;
  Graphik_hall8;
end;

procedure TForm1.Button18Click(Sender: TObject);
var  i,l:Integer; fmin:Extended; SerArr:SeriesArray; EditArr:EditArray;
begin
  for l:=1 to 7 do
  SerArr[l]:=Form4.FindComponent('Series'+IntToStr(l)) as TBarSeries;
  for l:=1 to 28 do
  EditArr[l]:=Form4.FindComponent('Edit'+IntToStr(l)) as TEdit;
  fmin:=10e8;
  Form4.ProgressBar1.Max:=100;
  GraphON_hall:=False;
  Form4.Show;
  for l:=1 to 7 do
  begin
    TBarSeries(Form4.FindComponent('Series'+IntToStr(l))).Clear;
    TPointSeries(Form5.FindComponent('Series'+IntToStr(l))).Clear;
  end;
  for l:=1 to 100 do
  begin
    Optimiz_hall8;
    for i:=1 to 6 do
    d1[i,l]:=Data[i];
    d1[7,l]:=func_hall8;
    // Разброс значений ***********************//
    Form5.Series7.AddXY(d1[7,l], l);
    Form5.Series1.AddXY(DATA[1], l);
    Form5.Series2.AddXY(DATA[2], l);
    Form5.Series3.AddXY(DATA[3], l);
    Form5.Series4.AddXY(DATA[4], l);
    Form5.Series5.AddXY(DATA[5], l);
    Form5.Series6.AddXY(DATA[6], l);
    //***************************************//
    if  d1[7,l]<fmin then
    begin
      fmin:=d1[7,l];
      Form4.Edit7.Text:=FloatToStr(fmin);
      for i:=1 to 6 do
      begin
      TEdit(Form4.FindComponent('edit'+IntToStr(i))).Text:=FloatToStr(Data[i]);
      end;
    end;
    Form4.ProgressBar1.Position:=l;
    Application.ProcessMessages(); // обрабатывает все сообщения поступившие
                                  // приложению
  end;
 Gistogram(d1,SerArr,7,100);
 Statistic(d1,EditArr,7,100);
 GraphON_hall:=True;
 ShowGraphics(EditArr, 6, Graphik_hall8);
end;

function TForm1.Interval:Boolean;   // рассчитывает подвижность и концентрацию
var i:Integer;             // 3-х типов носителей в интервале (1%; 150%)
    Stg3_pusto:Boolean;
    max,min:Extended;
begin
  min:=-0.5;
  max:=0.5;
  Stg3_pusto:=False;
  for i:=1 to 3 do
      if (StringGrid3.Cells[1,i]='') or (StringGrid3.Cells[2,i]='') then
         Stg3_pusto:=True;
      if Stg3_pusto then
      begin
        Interval:=True;
        Exit;
      end
      else
      begin
        Max_Value[1]:=RoundTo(StrToFloat(StringGrid3.Cells[2,3])+
        StrToFloat(StringGrid3.Cells[2,3])*min,-6);
        Max_Value[2]:=RoundTo(StrToFloat(StringGrid3.Cells[2,2])+
        StrToFloat(StringGrid3.Cells[2,2])*max,-3);
        Max_Value[3]:=RoundTo(StrToFloat(StringGrid3.Cells[2,1])+
        StrToFloat(StringGrid3.Cells[2,1])*max,-3);
        Max_Value[4]:=RoundTo(StrToFloat(StringGrid3.Cells[1,3])+
        StrToFloat(StringGrid3.Cells[1,3])*min,14);
        Max_Value[5]:=RoundTo(StrToFloat(StringGrid3.Cells[1,2])+
        StrToFloat(StringGrid3.Cells[1,2])*max,15);
        Max_Value[6]:=RoundTo(StrToFloat(StringGrid3.Cells[1,1])+
        StrToFloat(StringGrid3.Cells[1,1])*max,19);
        Min_Value[1]:=RoundTo(StrToFloat(StringGrid3.Cells[2,3])+
        StrToFloat(StringGrid3.Cells[2,3])*max,-3);
        Min_Value[2]:=RoundTo(StrToFloat(StringGrid3.Cells[2,2])+
        StrToFloat(StringGrid3.Cells[2,2])*min,-6);
        Min_Value[3]:=RoundTo(StrToFloat(StringGrid3.Cells[2,1])+
        StrToFloat(StringGrid3.Cells[2,1])*min,-6);
        Min_Value[4]:=RoundTo(StrToFloat(StringGrid3.Cells[1,3])+
        StrToFloat(StringGrid3.Cells[1,3])*max,15);
        Min_Value[5]:=RoundTo(StrToFloat(StringGrid3.Cells[1,2])+
        StrToFloat(StringGrid3.Cells[1,2])*min,14);
        Min_Value[6]:=RoundTo(StrToFloat(StringGrid3.Cells[1,1])+
        StrToFloat(StringGrid3.Cells[1,1])*min,18);
        Interval:=False;
      end;
      
end;


////////////////////////////////////////////////////////////////////////////
///////////////// КОНЕЦ "ХОЛЛ. МНОГОЗОННАЯ ПОДГОНКА" //////////////////////
////////////////////////////////////////////////////////////////////////////