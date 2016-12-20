% [20:54:54] Jaro Duraj: STM -> UI
% -poloha
% -ci inicializ�cia prebehla ok(ak posielam nie�o na UART, init prebehol v poriadku)
% [20:55:00 | Upraven� 20:55:16] Jaro Duraj: UI -> STM
% - mode auto/man
%  -man:  - polohu
%   -r�chlos� 
%   -hrani�n� polohy
%   -krokovanie
%   
%  -auto: -r�chlos�
%   -hrani�n� polohy
%   -krokovanie


%  Create and then hide the UI as it is being constructed.
   close all
   clear
   clc
   
   warning off
   delete(instrfindall)
   
   f = figure('Visible','off','Position',[300,300,600,300]);
   
   tabgp = uitabgroup(f,'Position',[0.55 0.05 0.3 0.70]);
   tab1 = uitab(tabgp,'Title','Manual');
   tab2 = uitab(tabgp,'Title','Auto');
   gp = uipanel(f,'Position',[0.55 0.79 0.3 0.15]);
   
   %currenttab = tabgp.SelectedTab; %return current sellected tab
   
   %  Construct the components.
   
   %elements inside tabs:
   
   %auto tab:
   hrychlost_a=uicontrol('Parent',tab2,'Style','edit','String','50','Position',[90,145,70,25]);
   htext1a = uicontrol('Parent',tab2,'Style','text','String','R�chlos�:','Position',[15,145,70,25]);
   
   hhranicne_a_upper=uicontrol('Parent',tab2,'Style','edit','String','60','Position',[90,45,70,25]);
   hhranicne_a_lower=uicontrol('Parent',tab2,'Style','edit','String','50','Position',[90,15,70,25]);
   htext2a = uicontrol('Parent',tab2,'Style','text','String','Hrani�n� polohy:','Position',[15,45,70,30]);
   
   hkrokovanie_a=uicontrol('Parent',tab2,'Style','edit','String','20','Position',[90,95,70,25]);
   htext3a = uicontrol('Parent',tab2,'Style','text','String','Krokovanie:','Position',[15,95,70,25]);
   
   %manual tab:
   hrychlost_m=uicontrol('Parent',tab1,'Style','edit','String','50','Position',[90,145,70,25]);
   htext1m = uicontrol('Parent',tab1,'Style','text','String','R�chlos�:','Position',[15,145,70,25]);
   
   hpoloha_m=uicontrol('Parent',tab1,'Style','edit','String','50','Position',[90,45,70,25]);
   htext2m = uicontrol('Parent',tab1,'Style','text','String','Poloha:','Position',[15,45,70,30]);
   
   hkrokovanie_m=uicontrol('Parent',tab1,'Style','edit','String','50','Position',[90,95,70,25]);
   htext3m = uicontrol('Parent',tab1,'Style','text','String','Krokovanie:','Position',[15,95,70,25]);
  
   %general objects
   hconnect = uicontrol('Style','togglebutton','String','Connect','Position',[20,235,70,50]);
   hport = uicontrol('Style','edit','String','COM1','Position',[120,240,70,25]);
   htext1 = uicontrol('Style','text','String','��slo portu:','Position',[120,270,70,15]);
   hvykonaj = uicontrol('Style','togglebutton','String','Vykonaj','Position',[220,235,70,50]); 
   hend = uicontrol('Style','togglebutton','String','Koniec','Position',[520,235,70,50]); 
   
   %auto/man panel:
   hauto = uicontrol('Style','radiobutton','String','Auto','Position',[350,255,50,15],'Value',1); 
   hman = uicontrol('Style','radiobutton','String','Manual','Position',[420,255,75,15],'Value',0); 
  
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
   
   %uhol=360; %stupnov
   %vykreslenie smeru:
   
   %uhol=90;
   %ciara=plot([0 0.5*cosd(uhol)],[0 0.5*sind(uhol)],'r','LineWidth',4);

   
   %potadeto nastav�m figure a prid�m do workspace
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   
   %init stuff:
   komunikacia=0;
   uhol=90;
   
   %nekone�n� cyklus na detekciu zmien a be�n� chod:
   while(1)
       % -- preklikanie medzi auto a man, ked kliknem na man
       % -- tak zru�� auto a opa�ne
       if get(hauto,'Value')==0 && get(hman,'Value')==1
           stav_man_auto=1;
       end
       if get(hauto,'Value')==1 && get(hman,'Value')==0
           stav_man_auto=2;
       end
       if get(hauto,'Value') == get(hman,'Value') 
           if stav_man_auto==1
               set(hauto,'Value',1);
               set(hman,'Value',0);
           end
           if stav_man_auto==2
               set(hauto,'Value',0);
               set(hman,'Value',1);
           end
           stav_man_auto=0;
       end
       drawnow %mus�m vykresli� lebo potom je busy a nerob� ni�
       
       %ak nekomunikujem tak zapnem komink�ciu
       if get(hconnect,'Value')==1 && komunikacia==0
           com=get(hport,'String');
           s = serial(com);
           set(s,'BaudRate',9600, 'Parity', 'none','FlowControl', 'none','DataBits',8,'StopBits',1);
           fopen(s);
           komunikacia=1;
           set(s,'Timeout',0.1);
           set(hconnect,'Value',0);
       end
       
       %ak komunikujem a chcem skon�i� program
       if get(hend,'Value')==1 && komunikacia==1
           fclose(s);
           komunikacia=0;
           delete(s)
           set(hend,'Value',0);
           break
       end
       if get(hend,'Value')==1 && komunikacia==0
           set(hend,'Value',0);
           break
       end
       
       %ak be�� komunik�cia vykreslujem hodnoty:
       if komunikacia==1
           try
               mess1=fread(s);
               mess2=fread(s);
               mess3=fread(s);
               if mess1==255;
                   uhol=bi2de([de2bi(mbin2) de2bi(mbin3)])/100;
               end
            catch
               uhol=90;
           end
           ciara=plot([0 0.5*cosd(uhol)],[0 0.5*sind(uhol)],'r','LineWidth',4); 
           drawnow
       end
       
       %ak stla��m tla�idlo vykonaj:
       if get(hvykonaj,'Value')==1
           %ak je man mod
           if get(hman,'Value')==1
               rychlost=str2num(get(hrychlost_m,'String'));
               poloha=de2bi(100*str2num(get(hpoloha_m,'String')));
               if(length(poloha)<16)
                   for k=length(poloha):1:16
                      poloha=[poloha 0];
                   end
               else
                   poloha=poloha(1:16);
               end    
               poloha1=bi2de(poloha(1:8));
               poloha2=bi2de(poloha(9:16));
               krokovanie=str2num(get(hkrokovanie_m,'String'));
               message=[2 rychlost poloha1 poloha2 krokovanie 0 0];
               fwrite(s,message,'uchar');
           end
           %ak je auto mod
           if get(hauto,'Value')==1
               rychlost=str2num(get(hrychlost_a,'String'));
               poloha=100*str2num(get(hhranicne_a_lower,'String'));
               poloha=de2bi(poloha);
               if(length(poloha)<16)
                   for k=length(poloha):1:16
                      poloha=[poloha 0];
                   end
               else
                   poloha=poloha(1:16);
               end
               poloha2a=bi2de(poloha(9:16));
               poloha1a=bi2de(poloha(1:8));
               krokovanie=str2num(get(hkrokovanie_a,'String'));
               poloha=100*str2num(get(hhranicne_a_upper,'String'));
               poloha=de2bi(poloha);
               if(length(poloha)<16)
                   for k=length(poloha):1:16
                      poloha=[poloha 0];
                   end
               else
                   poloha=poloha(1:16);
               end
               poloha2b=bi2de(poloha(9:16));
               poloha1b=bi2de(poloha(1:8));
               
               message=[1 rychlost poloha1a poloha2a krokovanie poloha1b poloha2b];
               fwrite(s,message,'uchar');
           end
           set(hvykonaj,'Value',0);
       end
       
       
       
       
   end
   hold off
   disp('koniec programu')
   
     