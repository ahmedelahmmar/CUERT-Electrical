num_samples = 50;

theta = linspace(0, 2*pi, num_samples);
sine_values = sin(theta + 0/3 * pi);
mapped_sine = (sine_values + 1) / 2;

fprintf('%d, ', round(mapped_sine .* 100));
