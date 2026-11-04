%% Лабораторная работа 2 matlab
% №1 Построить график функции y(x) = sin(x) на отрезке [0, 10] с шагом 0.1. Сделать линию красной пунктирной, добавить маркеры «звездочки», подписать оси и включить сетку
x = 0:0.1:10;            
y = sin(x);             
figure;                  
plot(x, y, 'r--*');      
xlabel('x');             
ylabel('y');             
title('График функции y = sin(x)'); 
grid on;                 

% №2 Требуется отобразить несколько функций либо на одном графике (используя hold on), либо разделить окно на несколько независимых областей с помощью команды subplot.
x = 0:0.1:10;
y1 = sin(x);
y2 = cos(x);

figure;
plot(x, y1, 'b');
hold on;
plot(x, y2, 'r');
hold off;
legend('sin(x)', 'cos(x)');
title('Несколько графиков на одном полотне');

% №3 Построить поверхность функции z(x,y) = sin(x)*cos(y) на области (x принадлежит [-3, 3], y принадлежит [-3, 3]).
[X, Y] = meshgrid(-3:0.1:3, -3:0.1:3);
Z = sin(X) .* cos(Y);

figure;
surf(X, Y, Z);
xlabel('x');
ylabel('y');
zlabel('z');
title('3D поверхность z = sin(x)*cos(y)');
colorbar;

% №4 Дан одномерный массив. С помощью цикла найти отрицательные элементы и заменить на ноль, положительные — возвести в квадрат.
vec = [-3, 5, -2, 7, 0, 4, -1];
for i = 1:length(vec)
    if vec(i) < 0
        vec(i) = 0;
    elseif vec(i) > 0
        vec(i) = vec(i)^2;
    end
end
disp('Измененный массив:');
disp(vec);

% №5 Найти сумму элементов квадратной матрицы, которые лежат выше главной диагонали.
A = [1, 2, 3; 4, 5, 6; 7, 8, 9];
[n, m] = size(A);
sum_above = 0;
for i = 1:n
    for j = i+1:m
        sum_above = sum_above + A(i, j);
    end
end
disp(['Сумма элементов выше главной диагонали: ', num2str(sum_above)]);

% №6  Написать программу для вычисления факториала числа N! с использованием цикла.
N = 5;
factorial_val = 1;
for i = 1:N
    factorial_val = factorial_val * i;
end
disp(['Факториал ', num2str(N), '! = ', num2str(factorial_val)]);

% №7 Дан одномерный массив из 10 случайных чисел. С помощью цикла определить характеристики этого массива (например, сумму, среднее, мин, макс).
vec = rand(1, 10);
sum_val = 0;
min_val = vec(1);
max_val = vec(1);

for i = 1:length(vec)
    sum_val = sum_val + vec(i);
    if vec(i) < min_val
        min_val = vec(i);
    end
    if vec(i) > max_val
        max_val = vec(i);
    end
end
mean_val = sum_val / length(vec);
fprintf('Сумма: %.2f, Среднее: %.2f, Мин: %.2f, Макс: %.2f\n', sum_val, mean_val, min_val, max_val);

% №8  Найти сумму элементов, которые больше заданного числа C.
vec = [1, 5, 2, 8, 3, 10];
C = 4;
sum_greater = 0;
for i = 1:length(vec)
    if vec(i) > C
        sum_greater = sum_greater + vec(i);
    end
end
disp(['Сумма элементов больше ', num2str(C), ': ', num2str(sum_greater)]);

% №9 Дана квадратная матрица размера N x N. Написать алгоритм, изменяющий элементы по правилу.
N = 3;
A = magic(N);
for i = 1:N
    for j = 1:N
        A(i,j) = A(i,j) * i * j;
    end
end
disp('Измененная матрица:');
disp(A);

% №10 Обнулить все элементы, стоящие на главной диагонали и ниже нее.
A = [1, 2, 3; 4, 5, 6; 7, 8, 9];
[n, m] = size(A);
for i = 1:n
    for j = 1:i
        A(i, j) = 0;
    end
end
disp('Матрица с обнуленной нижней частью:');
disp(A);

% №11 Дан массив чисел. Написать алгоритм с использованием циклов и ветвлений, который посчитает сумму всех положительных элементов массива и количество отрицательных.
vec = [1, -2, 3, -4, 5, -6, 0];
sum_pos = 0;
count_neg = 0;

for i = 1:length(vec)
    if vec(i) > 0
        sum_pos = sum_pos + vec(i);
    elseif vec(i) < 0
        count_neg = count_neg + 1;
    end
end

fprintf('Сумма положительных: %d\n', sum_pos);
fprintf('Количество отрицательных: %d\n', count_neg);
