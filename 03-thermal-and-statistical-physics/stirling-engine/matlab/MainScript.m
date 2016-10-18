clc;
clear all;

% Save the serial port name in comPort variable.

comPort = 'COM3';

% It creates a serial element calling the function "setupSerial"

if(~exist('serialFlag','var'))
    [arduino,serialFlag] = setupSerial(comPort);
end

% Time to create our plot window in order to visualize data collected 
% from serial port readings

if (~exist('h','var') || ~ishandle(h))
    h = figure(1);
    set(h,'UserData',1);
end

if (~exist('button','var'))
    button = uicontrol('Style','togglebutton','String','Stop',...
        'Position',[0 0 50 25], 'parent',h);
end

% After creating a system of two axis, an animated line object through 
% which the data will be plotted is also created
   
buf_len = 5000;
limits = [0 2]; % Uncomment this line to plot pressure
%limits = [0 360]; % Uncomment this line to plot angular position
    
l = animatedline;
axis([0, buf_len, limits]);
    
grid on;
data=zeros(1,3);
   
drawnow;


blah = readData(arduino);

blah = readData(arduino);

blah = readData(arduino);
i = 0;
tic;

while (get(button,'Value') == 0 )
    i = i+1;
    b = mod(i,buf_len);
    
    tx = strsplit(readData(arduino), ' ');
    
    bar = str2num(tx{1,1});
    temp = str2num(tx{1,2});
    deg = str2num(tx{1,3});
    
    data(i,1) = bar;
    data(i,2) = temp;
    data(i,3) = deg;
    data(i,4) = toc;
    addpoints(l,b,bar); % Uncomment this line to plot pressure
    %addpoints(l,b,deg); % Uncomment this line to plot angular position
    
    if (mod(i,10) == 0)
        drawnow;
    end
    
    if (b == buf_len -1)
        clearpoints(l);
    end
end

fclose(arduino);
delete(arduino);

