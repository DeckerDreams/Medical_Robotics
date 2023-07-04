output = [];

% a = a_dwn;
% a = a_vert;
% a = a_h;
% a = a_smile

for i = 2:length(a)
        if a(i-1,1) >= 0
            operater1 = "+";
        else 
            operater1 = "";
        end

        if a(i,1) >= 0
            operater2 = "+";
        else 
            operater2 = "";
        end



        b = "moveServo(BAS_SERVO, Base" + operater1 + a(i-1,1) + ", Base" + operater2+ a(i,1) + ");";
        
        
        
        
        if a(i-1,2) >= 0
            operater1 = "+";
        else 
            operater1 = "";
        end

        if a(i,2) >= 0
            operater2 = "+";
        else 
            operater2 = "";
        end
        s = "moveServo(SHL_SERVO, Shoulder" +operater1+ a(i-1, 2) + ", Shoulder" +operater2+ a(i, 2) + ");";
       







        if a(i-1,3) >= 0
            operater1 = "+";
        else 
            operater1 = "";
        end

        if a(i,3) >= 0
            operater2 = "+";
        else 
            operater2 = "";
        end
        e = "moveServo(ELB_SERVO, Elbow" +operater1+ a(i-1, 3) + ", Elbow" +operater2+  a(i, 3) + ");";
      





        if a(i-1,4) >= 0
            operater1 = "+";
        else 
            operater1 = "";
        end

        if a(i,4) >= 0
            operater2 = "+";
        else 
            operater2 = "";
        end
        w = "moveServo(WRI_SERVO, Wrist" +operater1+ a(i-1, 4) + ", Wrist" +operater2+ a(i, 4) + ");";

        delay = "delay(d);";
        
        output = [output;b;s;e;w;delay];
end

