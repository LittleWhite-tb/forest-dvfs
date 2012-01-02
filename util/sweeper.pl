#!/usr/bin/perl

use Env;
use Env qw(PATH LD_LIBRARY_PATH REST_PATH LIBREST_PATH PAPI_PATH POWER_PATH TIMER_PATH EVAL_LIBS REST_OUTPUT OUTPUT_FOLDER);
use Term::ANSIColor;
use Term::ANSIColor qw(:constants);
use Getopt::Long;
use Pod::Usage;
use File::Basename;



#Default options
my $activateColors = 1;	
my $activateHelp = '';
my $freqRange = "All";
my $freqId = '';
my $outputFolder = '';
my $applicationName = '';
my $verbose = '';

#number of application arguments
my $argc = $#ARGV+1;


main();

sub main
{
	parseOptions();
	setEnvValues();
	if($applicationName eq "SPEC" || $applicationName eq "spec" || $applicationName eq "Spec")
	{
		buildSPEC(@ARGV);
	}
	print $freqRange."\n";
	if($freqRange ne "restonly")
	{
		freqSwitching(@ARGV);
		launchREST(@ARGV);
	}
	else
	{
		launchREST(@ARGV);
	}
	
}

#Options Handeling
sub parseOptions
{
	GetOptions ("colors!"   	=> \$activateColors,
		    "help!" 		=> \$activateHelp,    
		    "freqrange=s"  	=> \$freqRange,      
	            "freqid=s"  	=> \$freqId,
		    "application=s"	=> \$applicationName,
	    	    "verbose!"		=> \$verbose,
		    "output_folder=s" 	=> \$outputFolder); 
	
	if($activateHelp == 1)
	{
		displayHelp();
	}
	
	
	if (isInRoot() != 1)
	{
		LogOutput(ERROR,"You have to be root");
		exit 1;	
	}

	#testing definition of options			
	if($applicationName eq '' || $freqRange eq '')
	{
		LogOutput(ERROR,"Wrong/No deffinition for options : --application, --freqrange\n");
		displayHelp();
	}
	if($freqRange eq "All")
	{
		LogOutput(ERROR,"--freqrange has to be defined as all not All\n");
		displayHelp();
	}
	elsif($freqRange ne "all" && $freqRange ne "restonly"  && $freqId eq "")
	{
		LogOutput(ERROR,"--freqrange is defined to $freqRange but --freqid is not defined\n");
		displayHelp();
	}
	elsif($freqRange ne "all" && $freqRange ne "restonly" && $freqId ne "")
	{
		@freqIds = split(/,/,$freqId);
		if( ($#freqIds+1) ne $freqRange)
		{
			my $nbIds = ($#freqIds+1);
			LogOutput(ERROR,"--freqrange is defined to $freqRange but --freqid has $nbIds freq id defined\n");
			displayHelp();
		}
	}
	
}

sub setEnvValues
{
	$ENV{REST_PATH} = "/opt/rest_modifications/Version1/";
	$ENV{PAPI_PATH} = "/usr/local/lib/libpapi.so";
	$ENV{LIBREST_PATH} = "/usr/local/lib/librest.so"; 
	$ENV{TIMER_PATH} = "/opt/rest_modifications/timer/timer.so";
	$ENV{POWER_PATH} = "/opt/rest_modifications/power/timer.so";
	$ENV{EVAL_LIBS} = "$ENV{TIMER_PATH};$ENV{POWER_PATH}";
	$ENV{ML_OUTPUT} = $outputFolder;
	$ENV{REST_OUTPUT} = $outputFolder;
	
	
	$ENV{LD_LIBRARY_PATH} = "$ENV{REST_PATH}:/opt/microlaunch/Libraries/power/:/opt/microlaunch/Libraries/power/esrv/:$ENV{PAPI_PATH}";
	
}


sub isInRoot
{
	my $root = execBashCmd("id -u");
	if($root == 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


sub launchML
{
	my $basename = shift(@_);
	my $nbCores = shift(@_);
	my $pathToRun = shift(@_);	
	my ($filename,$path) = fileparse($pathToRun);
	my $runSpecFile = $path."run_spec.sh";
	
	if ( -e  $runSpecFile)
	{
		execBashCmd("microlaunch --basename $basename --output-dir $ENV{ML_OUTPUT} --metarepetition 1 --repetition 1 --evallib \"$ENV{EVAL_LIBS}\" --nbprocess=$nbCores --execname=$runSpecFile --execargs \"@_\"");
	}
	elsif ( -e $pathToRun)
	{
		execBashCmd("microlaunch --basename $basename --output-dir $ENV{ML_OUTPUT} --metarepetition 1 --repetition 1 --evallib \"$ENV{EVAL_LIBS}\" --nbprocess=$nbCores --execname=$pathToRun --execargs \"@_\"");
	}
	else
	{
		LogOutput(ERROR,"There is no executable files at $pathToRun\n");
	}

}

sub buildSPEC
{	
	my $pathToRunSpec = shift(@_);
	my ($filename,$path) = fileparse($pathToRunSpec);
	my $pathToBuildSpec = $path."build_spec.sh";
	if ( -e $pathToBuildSpec)
	{
		LogOutput(OK,"Building SPEC with [@_] ...");
		execBashCmd("$pathToBuildSpec @_");
		LogOutput("","... Build successful\n");
	}
	else
	{
		LogOutput(ERROR,"There is no build_spec file in $pathToBuildSpec\n");
	}
}

sub freqSwitching
{
	my $nbProc = execBashCmd("cat /proc/cpuinfo | grep \"processor\" | wc -l");
	chomp $nbProc;
	my $freqs = execBashCmd("cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_available_frequencies");
	chomp $freqs;
	my @tmpfreqList = split (/ +/,$freqs);
	my @freqList = ();
	if ($#freqIds != -1)
	{
		foreach $id (@freqIds)
		{
			$freqList[$id]=$tmpfreqList[$id];
		}
	}
	else
	{
		@freqList = @tmpfreqList;
	}
	
	LogOutput(OK,"Launching $applicationName with [@_]\n");
	LogOutput(OK,"Running the benchmark on $nbProc cores\n");

	for ($i=0;$i<$nbProc;$i++)
	{
		if( -e "/sys/devices/system/cpu/cpu${i}/cpufreq/scaling_governor")
		{
			execBashCmd("echo userspace > /sys/devices/system/cpu/cpu${i}/cpufreq/scaling_governor");
		}
		else
		{
			LogOutput(ERROR,"/sys/devices/system/cpu/cpu${i}/cpufreq/scaling_governor does not exists");
			die;
		}
		
	}
	for ($j=0;$j<=$#freqList;$j++)
	{		
		for ($i=0;$i<$nbProc;$i++)
		{
			if( -e "/sys/devices/system/cpu/cpu${i}/cpufreq/scaling_setspeed")
			{
				execBashCmd("echo $freqList[$j] > /sys/devices/system/cpu/cpu${i}/cpufreq/scaling_setspeed");
			}
			else
			{
				LogOutput(ERROR,"/sys/devices/system/cpu/cpu${i}/cpufreq/scaling_setspeed does not exists");
				die;
			}
		}

		LogOutput(OK,"Frequency set to $freqList[$j] and userspace setting\n");
		LogOutput(OK,"Launching Microlaunch ....");
		launchML ($freqList[$j],$nbProc,@_);
		LogOutput("",".... Microlaunch Ended\n");
	}
	
	for ($i=0;$i<$nbProc;$i++)
	{
		if( -e "/sys/devices/system/cpu/cpu${i}/cpufreq/scaling_governor")
		{
			execBashCmd("echo userspace > /sys/devices/system/cpu/cpu${i}/cpufreq/scaling_governor");
		}
		else
		{
			LogOutput(ERROR,"/sys/devices/system/cpu/cpu${i}/cpufreq/scaling_governor does not exists");
			die;
		}
	}
	if ($#freqIds == -1)
	{
		LogOutput(OK,"Restore Ondemand setting\n");
		LogOutput(OK,"Launching Microlaunch ....");
		launchML ("ondemand",$nbProc,@_);
		LogOutput("",".... Microlaunch Ended\n");
	}
}

sub launchREST
{
	my $nbProc = execBashCmd("cat /proc/cpuinfo | grep \"processor\" | wc -l");
	chomp $nbProc;
	LogOutput(OK,"Launching REST on the application\n");
	LogOutput(OK,"Launching Microlaunch ....");
	launchML("naive",$nbProc,@_); 
;
	LogOutput("",".... Microlaunch Ended\n");
}

sub displayHelp
{
	pod2usage({-exitval => 1, -input => "./sweeper.perldoc", -output => \*STDOUT, -verbose => 1});
}

sub LogOutput
{
	my ($errorType,$message) = @_;
	if($errorType eq "WARNING")
	{
		if($activateColors == 1) { print BOLD, YELLOW,"[$errorType]> ",RESET; }
		else 
		{ 
			if($errorType ne ""){ print "[$errorType]> "; }
		}
	}
	elsif($errorType eq "USAGE")
	{
		if($activateColors == 1) { print BOLD, YELLOW,"[$errorType]> ",RESET; }
		else 
		{
			if($errorType ne ""){ print "[$errorType]> "; }
		}
	}
	elsif($errorType eq "ERROR")
	{
		if($activateColors == 1) { print BOLD, RED,"[$errorType]> ",RESET; }
		else 
		{ 
			if($errorType ne ""){ print "[$errorType]> "; }
		}
	}
	else
	{
		if($activateColors == 1) { print BOLD, GREEN,"[$errorType]> ",RESET; }
		else 
		{ 
			if($errorType ne ""){ print "[$errorType]> "; }
		}
	}
	print "$message";
}

sub execBashCmd
{
	my ($cmd) = @_;
	`touch error_LOG 2> /dev/null`;
	my $codeErr = `echo $?`;
	if ($codeErr > 0)
	{
		my $output = `$cmd 2> /dev/null`;
		return $output;
	}
	
	my $output = `$cmd 2> error_LOG`;
	my $codeErr = `echo $?`;
	if ($verbose == 1)
	{
		open (MYFILE, '>>sweeper_trace.txt');
		print MYFILE $output;
		close (MYFILE);
	}
	if ($codeErr > 0)
	{
		LogOutput(ERROR,`cat error_LOG`);
		`rm -f error_LOG`;
		die;
	}
	`rm -f error_LOG`;
	return $output;	
}
