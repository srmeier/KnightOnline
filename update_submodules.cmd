@echo off

ECHO.
ECHO ** OpenKO repository setup **
ECHO.
ECHO Preparing to initialise and update submodules
ECHO Finding git.exe...

set "GitDir="
where /q git
if not errorlevel 1 (
 	ECHO Found git.exe in PATH
 	goto init_and_update_submodules
)

ECHO git.exe not found in PATH, checking for GitHub Desktop directories...

set GithubDir=%LOCALAPPDATA%\GitHubDesktop
set "LocalGithubGitDir32=resources\app\git\mingw32\bin\"
set "LocalGithubGitDir64=resources\app\git\mingw64\bin\"
for /f "delims=" %%a in ('dir /b /ad /oN "%GithubDir%\app-*"') do (
	if exist "%GithubDir%\%%a\%LocalGithubGitDir64%git.exe" (
		set "GitDir=%GithubDir%\%%a\%LocalGithubGitDir64%"
	) else (
		if exist "%GithubDir%\%%a\%LocalGithubGitDir32%git.exe" (
			set "GitDir=%GithubDir%\%%a\%LocalGithubGitDir32%"
		)
	)
)

if "%GitDir%"=="" (
	ECHO Failed to update submodules: git.exe not found.
	ECHO Ensure that git is installed and placed in your PATH environment variable.
	PAUSE
	EXIT /B
)

ECHO Found git.exe in GitHub Desktop directory (%GitDir%)

:init_and_update_submodules
ECHO Initialising and updating submodules...
"%GitDir%git.exe" submodule update --init --recursive

ECHO.
ECHO All done! You can close this window now.
PAUSE
