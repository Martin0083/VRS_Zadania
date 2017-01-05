%  Create and then hide the UI as it is being constructed.
   close all
   clear
   clc
   
   warning off
   delete(instrfindall)
   disp('Spustenie GUI')
try   
   f = figure('Visible','off','Position',[300,300,600,350]);
   set(f,'Resize','off');
   
   tabgp = uitabgroup(f,'Position',[0.55 0.05 0.3 0.70]);
   tab1 = uitab(tabgp,'Title','Manual');
   tab2 = uitab(tabgp,'Title','Auto');
   gp = uipanel(f,'Position',[0.55 0.79 0.3 0.15]);
   gp2 = uipanel(f,'Position',[0.87 0.055 0.11 0.63]);
   gp3 = uipanel(f,'Position',[0.035 0.7 0.45 0.07]);
   %currenttab = tabgp.SelectedTab; %return current sellected tab
   
   %  Construct the components.
   
   %elements inside tabs:
   
   %auto tab:
   hrychlost_a=uicontrol('Parent',tab2,'Style','edit','String','50','Position',[90,145,70,25]);
   htext1a = uicontrol('Parent',tab2,'Style','text','String','Zmensenie casu jedneho kroku kazdu sekundu [ms]:','Position',[10,145,80,60]);
   
   hhranicne_a_upper=uicontrol('Parent',tab2,'Style','edit','String','330','Position',[90,45,70,25]);
   hhranicne_a_lower=uicontrol('Parent',tab2,'Style','edit','String','30','Position',[90,15,70,25]);
   htext2a = uicontrol('Parent',tab2,'Style','text','String','Hranicne polohy [�]:','Position',[15,45,70,30]);
   
   hkrokovanie_a=uicontrol('Parent',tab2,'Style','popup','String',{'0 (1/1 kroku)','1 (1/2 kroku)','2 (1/4 kroku)','3 (1/8 kroku)','4 (1/16 kroku)'},'Position',[90,95,70,25]);
   htext3a = uicontrol('Parent',tab2,'Style','text','String','Krokovanie:','Position',[15,95,70,25]);
   
   %manual tab:
   hrychlost_m=uicontrol('Parent',tab1,'Style','edit','String','50','Position',[90,145,70,25]);
   htext1m = uicontrol('Parent',tab1,'Style','text','String','Rychlost [otacky/min]:','Position',[15,145,70,30]);
   
   hpoloha_m=uicontrol('Parent',tab1,'Style','edit','String','50','Position',[90,45,70,25]);
   htext2m = uicontrol('Parent',tab1,'Style','text','String','Poloha [�]:','Position',[15,45,70,30]);
   
   hkrokovanie_m=uicontrol('Parent',tab1,'Style','popup','String',{'0 (1/1 kroku)','1 (1/2 kroku)','2 (1/4 kroku)','3 (1/8 kroku)','4 (1/16 kroku)'},'Position',[90,95,70,25]);
   htext3m = uicontrol('Parent',tab1,'Style','text','String','Krokovanie:','Position',[15,95,70,25]);
  
   %general objects
   hconnect = uicontrol('Style','togglebutton','String','Connect','Position',[20,280,70,50]);
   hport = uicontrol('Style','edit','String','COM4','Position',[120,285,70,25]);
   htext1 = uicontrol('Style','text','String','Cislo portu:','Position',[120,310,70,15]);
   hvykonaj = uicontrol('Style','togglebutton','String','Vykonaj','Position',[220,280,70,50]); 
   hend = uicontrol('Style','togglebutton','String','Koniec','Position',[520,280,70,50]); 
   htext_info = uicontrol('Style','text','String','Vitajte! Na zaciatku je potrebne kliknut na Connect','Position',[25,250,250,15]);
   
   
   htext0 = uicontrol('Style','text','String','0','Position',[255,115,20,15]);
   htext90 = uicontrol('Style','text','String','90','Position',[140,10,20,15]);
   htext180 = uicontrol('Style','text','String','180','Position',[25,115,20,15]);
   htext270 = uicontrol('Style','text','String','270','Position',[140,220,20,15]);
   
   
   %auto/man panel:
   hauto = uicontrol('Style','radiobutton','String','Auto','Position',[350,295,50,15],'Value',1); 
   hman = uicontrol('Style','radiobutton','String','Manual','Position',[420,295,75,15],'Value',0); 
  
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

   
   %potadeto nastavím figure a pridám do workspace
   %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   
   %init stuff:
   komunikacia=0;
   uhol=90;
   ciara=plot([0 0.5*cosd(uhol)],[0 0.5*sind(uhol)],'r','LineWidth',4);
   drawnow
   
   %nekone?ný cyklus na detekciu zmien a bežný chod:
   while(1)
       % -- preklikanie medzi auto a man, ked kliknem na man
       % -- tak zruší auto a opačne
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
       
       drawnow %musím vykresli? lebo potom je busy a nerobí ni?
       
       %ak nekomunikujem tak zapnem kominkáciu
       if get(hconnect,'Value')==1 && komunikacia==0
           com=get(hport,'String');
           s = serial(com);
           set(s,'BaudRate',115200, 'Parity', 'none','FlowControl', 'none','DataBits',8,'StopBits',1);
           fopen(s);
           komunikacia=1;
           set(s,'Timeout',0.1);
           set(hconnect,'Value',0);
           set(htext_info,'String','Pripojenie uspesne');
       end
       
       %ak komunikujem a chcem skon?i? program
       if get(hend,'Value')==1 && komunikacia==1
           set(htext_info,'String','Koncim...');
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
       
%ak beží komunikácia vykreslujem hodnoty:
       if komunikacia==1
           try
               mess=fread(s, 5, 'uchar');
               if mess(1)==255;
                   uhol = (bitsll(mess(3),8)+mess(2))/100;
               else
                   if mess(2)==255;
                      uhol = (bitsll(mess(4),8)+mess(3))/100;
                   else
                       if mess(3)==255;
                           uhol = (bitsll(mess(5),8)+mess(4))/100;
                       end
                   end
               end
            catch
               disp('neznámy uhol')
           end
           uhol=360-uhol;
           delete(ciara);
           ciara=plot([0 0.5*cosd(uhol)],[0 0.5*sind(uhol)],'r','LineWidth',4);
           drawnow
       end
       
       %limity nastavení:
       
       %limity polohy:
       if(str2num(get(hpoloha_m,'String'))>360)
           set(hpoloha_m,'String', '360');
       end
       if(str2num(get(hpoloha_m,'String'))<0)
           set(hpoloha_m,'String', '0');
       end
       
       %limity hraničných hodnôt:
       if(str2num(get(hrychlost_m,'String'))>100)
           set(hrychlost_m,'String','100');
       end
       if(str2num(get(hrychlost_m,'String'))<1)
           set(hrychlost_m,'String','100');
       end
       
       if(str2num(get(hrychlost_a,'String'))>1)
           set(hrychlost_a,'String','1');
       end
       if(str2num(get(hrychlost_a,'String'))<0.2)
           set(hrychlost_a,'String','0.2');
       end
       
       
       
       if(str2num(get(hhranicne_a_lower,'String'))>360)
           set(hhranicne_a_lower,'String', '360');
       end
       if(str2num(get(hhranicne_a_lower,'String'))<0)
           set(hhranicne_a_lower,'String', '0');
       end
       
       if(str2num(get(hhranicne_a_upper,'String'))>360)
           set(hhranicne_a_upper,'String','360');
       end
       if(str2num(get(hhranicne_a_upper,'String'))<0)
           set(hhranicne_a_upper,'String','0');
       end
       
       if(str2num(get(hhranicne_a_upper,'String'))<str2num(get(hhranicne_a_lower,'String')))
           set(hhranicne_a_lower,'String',get(hhranicne_a_upper,'String')); 
       end
       
       
       
       
       %ak stla?ím tla?idlo vykonaj:
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
               krokovanie=get(hkrokovanie_m,'Value')-1;
               message=[2 rychlost poloha1 poloha2 krokovanie 0 0];
               fwrite(s,message,'uchar');
           end
           %ak je auto mod
           if get(hauto,'Value')==1
               rychlost=10*str2num(get(hrychlost_a,'String'));
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
               krokovanie=get(hkrokovanie_a,'Value')-1;
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
       %pause(0.01)
   end
   hold off
   disp('Koniec programu')
   close Figure 1
catch
    disp('Error')
    set(htext_info,'String','Nastalo zlyhanie programu. Je potrebny restart GUI');
    clear
end
