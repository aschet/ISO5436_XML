% Compile the Mex files for openGPS X3P import/export
% (C) by Georg Wiora (NanoFocus AG)
% 11-Dec-2008

%% Unload all libraries
clear all libraries

% Fileseparator
fs=filesep;
% go one level up
up=['..',fs];

% Get current directory
d = [pwd(),fs];

% Debug flag
debug='-g '

%% Build path names

% Installation path of x3p lib
InstallX3P = [d,up,up,up,up,'install',fs]
% Include directory for x3p
IDirX3P = ['-I"',InstallX3P,'include',fs,'" ']
% Library directory for x3p
LDirX3P = ['-L"',InstallX3P,'lib',fs,'" ',...
           '-L"',InstallX3P,'bin',fs,'" ']
% Set library name of x3P DLL
LibX3P= '-lISO5436_2_XML_S '

% Path of x3p dll
LibX3PPath = [InstallX3P,'bin',fs,'ISO5436_2_XML.dll']
           
%%

% Search installation of Codesynthesis XSD-tool and Xerces library
% This will only work on windows
if ispc()
  % Get system PATH environment variable and look for codesynthesys
  sp = getenv('PATH');
  % Separate paths
  spk = strfind(lower(sp), 'codesynthesis');
  % Get first occurence
  if numel(spk)<1
    error('Please specifiy the include and library path of codesynthesys Xerces library and remove this error message!');
    IDirCS='Please specify path to include files here';
    LDirCS='Please specify path to library directory here';
  else
    spk = spk(1);
    % Find separator before location
    st=1;
    for i=spk:-1:1
      if sp(i) == ';'
        st = i+1;
        break;
      end
    end
    % Find separator after location
    en=numel(sp);
    for i=spk:en
      if sp(i) == ';'
        en = i-1;
        break;
      end
    end
    % grep the bin path of codesynthesys
    DirCS = [sp(st:en),fs];
    % Set include and library path
    IDirCS=['-I"',DirCS,up,'include',fs,'" ']
    % ToDo: We stick to vc8 here, this may have to be changed
    warning('Assuming you are using Visual C8.0. If this is not the case please change the version of Codysynthesys libraries!');
    LDirCS=['-L"',DirCS,up,'lib',fs,'vc-8.0',fs,'" ',...
            '-L"',DirCS,'" ']
    % Path to xerces DLL
    LibXercesPath = [DirCS,'xerces-c_2_7_vc80.dll']
  end
else
  error('Please specifiy the include and library path of codesynthesys Xerces library and remove this error message!');
  IDirCS='Please specify path to include files here';
  LDirCS='Please specify path to library directory here';
end

%% Create compile commands
cmex=['mex ',debug,IDirX3P,LDirX3P,LibX3P,IDirCS,LDirCS,'COMPFLAGS="$COMPFLAGS /Zc:wchar_t" '];
cmexopenX3P=[cmex,'openX3P.cpp'];
cmexwriteX3P=[cmex,'writeX3P.cpp'];

%% compile
disp(cmexopenX3P);
eval(cmexopenX3P)

disp(cmexwriteX3P);
eval(cmexwriteX3P)

%% Copy the dlls to the same directory as the mex files
copyfile(LibX3PPath,'.')
copyfile(LibXercesPath,'.')
