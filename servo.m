%% Servo Control
% This script contains the functions required to control a single servo motor

classdef servo
   properties (SetAccess = protected)
      % Position of the servo
      position;
      
      % Serial port
      serial;
      
      % Default servo position
      default_pos = 70;
      
      % Max and min servo positions
      max = 140;
      min = 40;
   end
   methods
      % Constructor class, initializes the serial variable, and position
      function obj = servo(ard)
         obj.serial = ard;
         obj.position = obj.default_pos;
      end
      
      function output = check_input(obj, input)
          % This function ensures that the servo input does not exceed the
          % predefined limits
          
          % Makes sure that the input is within the earlier defined
          % bounds
          if input > obj.max
              input = obj.max;
          elseif input < obj.min
              input = obj.min;
          end
          
          output = input;
      end
      
      function send_pos(obj, pos)
          % This is the code for sending the servo position over the
          % Virtual Com Port (VCP)
          ard = obj.serial;
          TXBuf = zeros(2,1);
          P = typecast(uint16(pos),'uint8');
          TXBuf(1) = P(2);
          TXBuf(2) = P(1);

          for i = 1:2
              fwrite(ard, TXBuf(i));
          end
      end
      
      function obj = change_pos(obj, pos)
          % This function receives the desired servo position, checks
          % it, and then sends it to the Arduino over obj.serial
          
          % Ensures value is within limits
          pos = check_input(obj, pos);
          
          % Sends the position over the virtual com port (VCP) to the Arduino / other MCU
          send_pos(obj, pos);
          
          % Updates the class' servo position value
          obj.position = pos;
      end
   end
end
