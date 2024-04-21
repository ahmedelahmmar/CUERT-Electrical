LUT_SIZE = 64;
AUTORELOAD = 100;

theta = linspace(0, pi, LUT_SIZE);
sine_values = sin(theta + 0/3 * pi);
mapped_sine = (sine_values + 1) / 2;

fprintf('%d, ', round(sine_values .* AUTORELOAD));
