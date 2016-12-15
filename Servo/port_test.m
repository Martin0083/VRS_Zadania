s = serial('COM1');
set(s,'BaudRate',9600, 'Parity', 'none','FlowControl', 'none','DataBits',8,'StopBits',1,'Terminator','CR');
fopen(s);

while(1)
    %hex2dec('FF')==255;
    mhex=fscanf(s)
    try
        mbin=hexToBinaryVector(mhex)
    catch
        mbin=0
    end
    

end

fclose(s);