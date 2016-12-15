% [20:54:54] Jaro Duraj: STM -> UI
% -poloha
% -ci inicializ·cia prebehla ok(ak posielam nieËo na UART, init prebehol v poriadku)
% [20:55:00 | UpravenÈ 20:55:16] Jaro Duraj: UI -> STM
% - mode auto/man
%  -man:  - polohu
%   -r˝chlosù 
%   -hraniËnÈ polohy
%   -krokovanie
%   
%  -auto: -r˝chlosù
%   -hraniËnÈ polohy
%   -krokovanie


%  Create and then hide the UI as it is being constructed.
   close all
   clear
   clc
   
   f = figure('Visible','off','Position',[300,300,600,300]);
   
   tabgp = uitabgroup(f,'Position',[0.55 0.05 0.3 0.70]);
   tab1 = uitab(tabgp,'Title','Auto');
   tab2 = uitab(tabgp,'Title','Manual');
   gp = uipanel(f,'Position',[0.55 0.79 0.3 0.15]);
   
   %currenttab = tabgp.SelectedTab; %return current sellected tab
   
   %  Construct the components.
   
   %elements inside tabs:
   
   %manual tab:
   hrychlost_m=uicontrol('Parent',tab2,'Style','edit','String','','Position',[90,145,70,25]);
   htext1m = uicontrol('Parent',tab2,'Style','text','String','R˝chlosù:','Position',[15,145,70,25]);
   
   hhranicne_m_upper=uicontrol('Parent',tab2,'Style','edit','String','','Position',[90,45,70,25]);
   hhranicne_m_lower=uicontrol('Parent',tab2,'Style','edit','String','','Position',[90,15,70,25]);
   htext2m = uicontrol('Parent',tab2,'Style','text','String','HraniËnÈ polohy:','Position',[15,45,70,30]);
   
   hkrokovanie_m=uicontrol('Parent',tab2,'Style','edit','String','','Position',[90,95,70,25]);
   htext3m = uicontrol('Parent',tab2,'Style','text','String','Krokovanie:','Position',[15,95,70,25]);
   
   %auto tab:
   hhranicne_a_upper=uicontrol('Parent',tab1,'Style','edit','String','','Position',[90,95,70,25]);
   hhranicne_a_lower=uicontrol('Parent',tab1,'Style','edit','String','','Position',[90,65,70,25]);
   htext1a = uicontrol('Parent',tab1,'Style','text','String','HraniËnÈ polohy:','Position',[15,95,70,30]);
   
   hkrokovanie_a=uicontrol('Parent',tab1,'Style','edit','String','','Position',[90,145,70,25]);
   htext2a = uicontrol('Parent',tab1,'Style','text','String','Krokovanie:','Position',[15,145,70,25]);
   
   %general objects
   hconnect = uicontrol('Style','togglebutton','String','Connect','Position',[20,235,70,50]);
   hport = uicontrol('Style','edit','String','','Position',[120,240,70,25]);
   htext1 = uicontrol('Style','text','String','»Ìslo portu:','Position',[120,270,70,15]);
   hvykonaj = uicontrol('Style','pushbutton','String','Vykonaj','Position',[220,235,70,50]); 
   
   %auto/man panel:
   hauto = uicontrol('Style','radiobutton','String','Auto','Position',[350,255,50,15]); 
   hman = uicontrol('Style','radiobutton','String','Manual','Position',[420,255,75,15]); 
  
   ha = axes('Units','Pixels','Position',[50,30,200,185]); 
   %align([hconnect,hvykonaj,hautoman,tabgp],'Center','None');
   
   % Make the UI visible.
   f.Visible = 'on';
   
   %vykreslenie kruhu:
   x=-0.5:0.01:0.5;
   y=sqrt(0.5.^2-(x).^2);
   
   hold on
   axis off
   grid on
   
   kruhA=plot(x,y,'r','LineWidth',4);
   kruhB=plot(x,-y,'r','LineWidth',4);
   
   uhol=360; %stupnov
   %vykreslenie smeru:
   
   for uhol=0:5:360
        ciara=plot([0 0.5*cosd(uhol)],[0 0.5*sind(uhol)],'r','LineWidth',4);
        pause(0.0001)
        delete(ciara)
   end
   uhol=90;
   ciara=plot([0 0.5*cosd(uhol)],[0 0.5*sind(uhol)],'r','LineWidth',4);
   
   
   
   
   
   
   
   
   hold off
   
   
       
     