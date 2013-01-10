function parameterPlots(param)
% parameterPlots(param)
%   Generates report plots for the various filters used in the receiver for the
%   specified modulation parameters.

    p = param;

    % PLL (closed-loop)
    z = tf('z', 1);
    phaseAccum = 1 / (1 - z^-1);
    loopFilt = tf(p.PhaseB, p.PhaseA, 1);
    phaseAccum = p.pllLoopGain * phaseAccum;
    step(phaseAccum*loopFilt / (1 + phaseAccum*loopFilt), 100);
    xlabel('Symbols');
    title('PLL Closed-Loop Step Response');

    % AGC filter
    figure;
    g = tf(p.AmplB, p.AmplA, 1);
    step(g);
    title('AGC Filter Step Response');
    xlabel('Symbols');

    % Softbit filter
    figure;
    g = tf(p.SoftBitB, p.SoftBitA, 1);
    step(g);
    title('Softbit Filter Step Response');
    xlabel('Symbols');

end
