% Create sample x3p files from a given dataset

%% Read sample file
%s=openNMS('1-euro-star.nms')
% Convert tom meter
u = s.u/1e6;
v = s.v/1e6;
z = s.z/1e6;


%% Write x3p file
meta.Date = '2008-08-25T14:14:00.0+02:00';
meta.Creator = 'Dr. Georg Wiora, NanoFocus AG';
meta.Instrument_Manufacturer = 'NanoFocus AG';
meta.Instrument_Model = 'µsurf explorer';
meta.Instrument_Serial = 'not available';
meta.Instrument_Version = 'Software V6.7, Instrument Version D';
meta.CalibrationDate = '2008-08-25T13:59:21.4+02:00';
meta.ProbingSystem_Type = 'NonContacting';
meta.ProbingSystem_Identification = 'Olympus 20x0.6';
meta.Comment = 'Confocal Measurement of star structure on a 1 Euro coin. Objective 20x0.6';

pinfo = writeX3P('1-euro-star.x3p','SUR',u,v,z,meta);

%% Add an offset and a rotation
rotx=10; % Rotation angle around x (degree)
R = [1,0,0;...
     0,cosd(rotx),sind(rotx);...
     0,-sind(rotx),cosd(rotx)];

pinfo = writeX3P('1-euro-star_offset_rot.x3p','SUR',u,v,z,meta,'rotation',R,'translation',[1e-6,5e-3,1e0])

%% Create missing points
zmissing = z;
zmissing(450:470,500:510) = NaN;
pinfo = writeX3P('1-euro-star_missing.x3p','SUR',u,v,zmissing,meta)

%% Write absolute x and y coordinates
rotx=10; % Rotation angle around x (degree)
R = [1,0,0;...
     0,cosd(rotx),sind(rotx);...
     0,-sind(rotx),cosd(rotx)];

u3d = repmat(u,970,1);
v3d = repmat(v,1,970);
   
pinfo = writeX3P('1-euro-star_offset_rot_absxy.x3p','SUR',u3d,v3d,z,meta,'rotation',R,'translation',[1e-6,5e-3,1e0])

%% Add a small jitter (500 nm) on x and y coordindates to test the interpolation
u3dj = u3d + rand(size(u3d,1),size(u3d,2))*5e-7;
v3dj = v3d + rand(size(u3d,1),size(u3d,2))*5e-7;

pinfo = writeX3P('1-euro-star_offset_rot_absxy_jitter.x3p','SUR',u3dj,v3dj,z,meta,'rotation',R,'translation',[1e-6,5e-3,1e0])


%% Add a second layer with a 100 µm offset
z2 = repmat(z,[1,1,2]);
z2(:,:,2) = z2(:,:,2) + 100e-6;
pinfo = writeX3P('1-euro-star_2_layer.x3p','SUR',u,v,z2,meta)

%% Create a profile
prof = z(500,:);
pinfo = writeX3P('1-euro-star_profile.x3p','PRF',u,v(500),prof,meta)
%% Create a two layer profile
prof = z(500,:);
pinfo = writeX3P('1-euro-star_profile_2_layer.x3p','PRF',u,v(500),prof,meta)

%% Create a profile with rotation and offset
prof = z(:,500);
pinfo = writeX3P('1-euro-star_profile_rot_trans.x3p','PRF',u(500),v,prof,meta,'rotation',R,'translation',[1e-6,5e-3,1e0])
