d = load('data.txt');

f = d(:,1);
rng = d(:,2);
int = d(:,3);

theta = [0 : 1 : 359]'*pi/180;

x = rng.*cos(theta);
y = rng.*sin(theta);

%idxg = find(rng>0.6 & rng<6.0 & f==0);
idxg = find(f==0);

figure;
hold on;
plot(x,y,'.');
plot(x(idxg),y(idxg),'r.');
axis equal;
grid on;
 
