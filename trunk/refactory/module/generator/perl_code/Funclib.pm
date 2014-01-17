package Funclib;
use strict;
use Digest::MD5  qw/ md5_hex /;
use POSIX qw(strftime);
use Time::Local;
use Encode;
my $SN = "Funclib";
my $LOG_LEVEL = 16;
sub ReadConfValueForName
{
	my ($conffile,$confname) = @_;
	if(!open(FH,"<$conffile"))
        {
                FATAL_LOG("open conf file fail:$conffile");
                return 0;
        }
	my $line;
        while($line = <FH>)
        {
                chomp($line);
                next if($line =~ /^\s*#/);
                if($line =~ /^\s*$confname\s*:\s*(\S.*?)\s*$/)
                {
			my $value = $1;
			close(FH);
			NOTICE_LOG("ReadConfValueForName read conf:[$confname]--->[$value]");
			return $value;
                }
        }
	close(FH);
	return "";
}
sub LoadConfPairFromFile
{
	my ($conffile,$confres) = @_;
#	%$confres = ();
	if(!open(FH,"<$conffile"))
	{
		FATAL_LOG("$SN LoadConfPairFromFile open conf file fail! file:$conffile");
		return -1;
	}
	my $line; my $num = 0;
        while($line = <FH>)
        {
                chomp($line);
                next if($line =~ /^\s*#/);
		$line =~ $1 if($line =~ /^(.+?)\s*$/);
                if($line =~ /^\s*(\S+)\s*:\s*(\S.*?)\s*$/)
                {
			my ($confname,$value) = ($1,$2);
			$confres->{$confname} = $value;
                        NOTICE_LOG("LoadConfPairFromFile read conf:[$confname]--->[$value]");
			$num++;
                }
        }
        close(FH);
	return $num;
}
sub ReadConfItem
{
	my ($confname,$conffile,$confres) = @_;
	if(!open(FH,"<$conffile"))
        {
                FATAL_LOG("open conf file fail:$conffile");
                return 0;
        }
	my $isok = 0;
	my $itemnum = 0;
	#NOTICE_LOG("ReadConfItem read conf:$confname file:$conffile");
	my $line;
	 while($isok == 0 && ($line = <FH>))
        {
                chomp($line);
		#print "read conf: name:$confname line:$line\n";
                next if($line =~ /^\s*#/);
                if($line =~ /^\s*$confname\s*:\s*(\S.*?)\s*$/)
                {
                        my $name = $confname;
                        my $value = $1;
                        $confres->{$name} = $value;
                        NOTICE_LOG("ReadConfItem read conf:[$name]--->[$value]");
			$isok = 1;
			last;
                }
        }
	close(FH);
	return 1 if($isok);
	WARN_LOG("ReadConfItem read fail! name:$confname");
	return 0;
}
sub GetDivInt
{
	my ($a,$b) = @_;
	return 0 if($a== 0 || $b == 0);
	my $r  = $a/$b;
	$r = 0 if($r < 0.0001);
	$r = $1 if($r =~ /^(\d+)\./);
	return $r;
}
sub RunCMD
{
	my ($cmd) = @_;
	NOTICE_LOG("RunCMD START: $cmd");
	system("$cmd");
	NOTICE_LOG("RunCMD END: $cmd");
	return $cmd;
}
sub SaveContentToFile
{
	my ($filename,$content) = @_;
	open(FH,">$filename") or die("SaveContentToFile open file fail! file:$filename");
	NOTICE_LOG("SaveContentToFile save to content to file:$filename");
	print FH "$content";
	close(FH);
}
sub GetUrlFileType
{
	my ($url) = @_;
	my $cgi = $url;
	$cgi = $1 if($url =~ /^([^?]+?)\?/);
	if($cgi =~ /\/[^\/]+?(\.[a-zA-Z0-9\-_]+)$/)
	{
		return $1;
	}elsif($cgi =~ /\/[^\/]+?(\.[a-zA-Z0-9\-_]+)[\?;]/)
	{
		return $1;
	}
	return "";
}
sub GetFileMd5
{
	my ($file) = @_;
	return 0 if(!(-e $file));
	my $md5 = `md5sum $file`;
	chomp($md5);
	$md5=$1 if($md5 =~ /^\s*(\S+)\s*(\S+)\s*$/);
	return $md5;
}
sub MakeReleaseFileMd5
{
	my ($srcfile) = @_;
	my $str = GetFileMd5($srcfile);
	WriteContToFile($str,"$srcfile.md5");
	NOTICE_LOG("$SN MakeReleaseFileMd5 srcfile:$srcfile outfile:$srcfile.md5");
}
sub GetHostName
{
	my $host = `hostname`;
	chomp($host);
	return $host;
}
sub SortBigFile
{
	my ($file) = @_;
	my $size = `du -b $file`;
#	print "sizestr:$size\n";
	$size = $1 if($size =~ /^\s*(\d+)\s+/);
#	print "get size:$size\n";
	my $gbyte = 1024*1024*100;
#	print "size:$gbyte file:$file\n";
	if($size <$gbyte)
	{
#		print "$SN SortBigFile small file size:$size\n";
		`sort $file>$file.sort`;
		`mv $file.sort $file`;
		return 1;
	}
#	DEBUG_LOG("$SN SortBigFile size:$size file");
	my ($path,$name) = ParseFilePathName($file);
	DEBUG_LOG("$SN SortBigFile size:$size path:$path name:$name file:$file");
#	DEBUG_LOG("$SN SortBigFile split file:$file");
	my $cmd = "cd $path; split -d -l 2000000 $name $name.split";
	DEBUG_LOG("$SN SortBigFile split file:$file cmd:$cmd");
	system("$cmd");
	my $cont = `ls $path/$name.split*`;
	my @arr = split(/\s+/,$cont);
#	DEBUG_LOG("$SN SortBigFile get split filelist:".@arr);
	my $mergecmd = "sort -m -T $path ";
	foreach my $spfile (@arr)
	{
		$cmd = "sort -T $path $spfile>$spfile.sort";
		DEBUG_LOG("$SN SortBigFile sort split file:$spfile cmd:$cmd");
		system("$cmd");
		$mergecmd .= " $spfile.sort";
	}
	$mergecmd .= ">$file.sort";
	DEBUG_LOG("$SN SortBigFile sort merge file,cmd:$mergecmd");
	system("$mergecmd");
	`rm -rf $path/$name.split* `;
	`mv $file.sort $file`;
	return 1;
	
}
sub SendSMS
{
	my ($mobile,$cont) = @_;
	my @arr = split(/\s+/,$mobile);
	my $host = GetHostName();
	foreach my $no (@arr)
	{
		next if($no =~ /^\s*$/);
		my $cmd = "/bin/gsmsend-script \"$no\"@\"$host:$cont\"";
		NOTICE_LOG("SendSMS CMD:$cmd");
		`cmd`;
	}
}
sub GetPureXmlCont
{
	my ($cont) = @_;
	if($cont =~ /^\s*<!\[CDATA\[(.+?)\]\]/)
	{
		return $1;
	}else
	{
		return $cont;
	}
}
sub PrintLineToFile
{
	my ($fh,$line) = @_;
	print $fh "$line";
}
sub GetDate
{
        my $dt = `date +%Y-%m-%d`;
        chomp($dt);
        return $dt;
}
sub GetFileBakDate
{
	my $dt = `date "+%Y%m%d%H%M%S"`;
	chomp($dt);
	return $dt;
}
sub GetLineNum
{
	my ($file) = @_;
	return 0 if(! (-e $file));
	my $cont = `wc -l $file`;
	if($cont =~ /^\s*(\d+)\s+/)
	{
		my $num = $1;
		NOTICE_LOG("$SN GetLineNum:$num file:$file ");
		return $num;
	}
	return 0;
}
sub SplitFile
{
	my ($srcfile,$outpath,$maxnum) = @_;
	$maxnum = 1 if($maxnum <1);
	my $totlinenum = GetLineNum($srcfile);
	my $numpfile = $totlinenum/$maxnum;
	my %res = ();
	$res{"filenum"} = 0;
	if($numpfile < 1)
	{
        	$numpfile = $totlinenum;
	}
	if(!open(FH,"<$srcfile"))
	{
		FATAL_LOG("$SN SplitFile open file fail! srcfile:$srcfile");
		return %res;
	}
	my $fnum = 0;
	my $fname = GetFileName($srcfile);
	my $tmpfile = "$outpath/$fname-$fnum";
	my $itemnum = 0;
	$numpfile += 5;
	NOTICE_LOG("$SN SplitFile maxnum:$maxnum numperfile:$numpfile totlinenum:$totlinenum");
	if(!open(TMPFH,">$tmpfile"))
	{
		FATAL_LOG("$SN SplitFile open tmp file fail! file:$tmpfile");
		return %res;
	}
	$res{"file$fnum"} = $tmpfile;
	while(my $line = <FH>)
	{
		chomp($line);
		print TMPFH "$line\n";
		$itemnum++;
		if($itemnum>=$numpfile)
		{
			close(TMPFH);
			NOTICE_LOG("$SN SplitFile add file$fnum:$tmpfile");
			$fnum++;
			$tmpfile = "$outpath/$fname-$fnum";
			$itemnum = 0;
			if(!open(TMPFH,">$tmpfile"))
        		{
                		FATAL_LOG("$SN SplitFile open tmp file fail! file:$tmpfile");
                		return %res;
        		}
			$res{"file$fnum"} = $tmpfile;
		}
		
	}
	if($itemnum>0)
	{
		close(TMPFH);
		$fnum++;
	}
	close(FH);
	$res{"filenum"} = $fnum;
	return %res;
}
sub GetFileName
{
	my ($file) = @_;
	my $name = "unknown";
	if($file =~ /\/([^\/]+)\s*$/)
	{
		$name = $1;
	}
	DEBUG_LOG("$SN GetFileName name:$name file:$file");
	return $name;
}
sub ParseFilePathName
{
	my ($file) = @_;
	return (".",$1) if($file=~ /^\.?([^\/]+)$/);
	return ("$1/",$2) if($file =~ /^(.+?)\/([^\/]+)$/);
	return (".",$file);
}
sub GetPercent
{
	my ($a,$b) = @_;
	return 0 if($b == 0);
	my $rate = 100 * $a/$b;
	$rate = "0.0" if($rate <0.0001);
	$rate = $1 if($rate =~ /^\s*(\d+\.\d\d\d)/);
	return "$rate%";
}
sub BackupFileToPath
{
	my ($file,$path) = @_;
	return 0 if(! (-e $file));
        my $dt = GetFileBakDate();
	my $fname = GetFileName($file);
	my $bakfile = "$path/$fname.$dt";
	`cp $file $bakfile`;
	return $bakfile;
}
sub BackupFile
{
	my ($file) = @_;
	return 0 if(! (-e $file));
	my $dt = GetFileBakDate();
	my $bakfile = "$file.$dt";
	`cp $file $bakfile`;
	return $bakfile;
}
sub GetFileSize
{
	my ($file) = @_;
	return 0 if(!(-e $file));
	my $cont = `wc -c $file`;
	if($cont =~ /^\s*(\d+)\s+/)
	{
		return $1;
	}
	return 0;
}
sub MailTo
{       
        my($tit,$cont,$file,$MAIL_TO) = @_;
        my $body = "";
        $body = `cat $file` if(-e $file);
        $cont .= "\n$body";
        my $dt = GetDate();
        $cont =~ s/"/\\"/g;
        my $cmd = "echo \"$cont\" |  mail -s \"$tit-($dt)\" $MAIL_TO";
#	NOTICE_LOG("send mail cmd:[$cmd]");
        `$cmd`;
        return 1;
}
sub GetFileList
{
	my ($patt) = @_;
	my $list = `ls $patt`;
	my @arr = split(/\s+/,$list);
	return @arr;
}
sub GetFileListIncSubDir
{
	my ($pathpatt,$filepatt) = @_;
	my $cont = `ls -R $pathpatt`;
	
	
}
sub LoadConfFile
{
	my ($conffile) = @_;
	my %conf = ();
	$conf{"isok"} = 0;
	if(!open(FH,"<$conffile"))
	{
		FATAL_LOG("open conf file fail:$conffile");
		return %conf;
	}
	my $itemnum = 0;
	while(<FH>)
	{
		my $line = $_;
		chomp($line);
		$line = $1 if($line =~ /^(.+?\S)\s*$/);
		next if($line =~ /^\s*#/);
		next if($line =~ /^\s*$/);
		if($line =~ /^\s*(\S+)\s*:\s*(\S.*)\s*$/)
		{
			my $name = $1;
			my $value = $2;
			$conf{$name} = $value;
			NOTICE_LOG("read conf No:$itemnum :[$name]--->[$value]");
			$itemnum++;
		}elsif($line =~ /^\s*(\S+)\s*:\s*$/)
		{
			my $name =$1;
			my $value = "";
			$conf{$name} = $value;
			NOTICE_LOG("read conf No:$itemnum :[$name]--->[$value]");
			$itemnum++;
		}else
		{
			FATAL_LOG("illegal conf line,must fix! line:[$line]");
			return %conf;
		}
	}
	close(FH);
	$conf{"isok"} = 1 if($itemnum>0);
	return %conf;
	
}
sub GetLogDate
{
	my $dt = strftime "%Y-%m-%d %H:%M:%S", localtime(time);
#	my $dt = `date "+%Y-%m-%d %H:%M:%S"`;
#	chomp($dt);
	return $dt;
}
sub GetTimeInSec
{
	my $sec = `date +%s`;
	chomp($sec);
	return $sec;
}
sub WriteContToFile
{
	my ($cont,$file) = @_;
	if(!open(FH,">$file"))
	{
		print "Funclib::WriteContToFile open file fail! file:$file";
		return 0;
	}
	print FH "$cont";
	close(FH);
	return 1;
}
sub CreatSign
{
	my ($str) = @_;
#	print "CreatSign srcstr:[$str]\n";
	$str = $1 if($str =~ /^\s*(\S.*?)\s*$/);
	my $sign = md5_hex($str);
#	print "CreatSign [$sign]<---[$str]\n";
	return $sign;
}
sub PrintLog
{
	my ($flag,$cont) = @_;
	my $dt = GetLogDate();
	print "$dt $flag $cont\n";
}
sub NOTICE_LOG
{
	my ($cont) = @_;
	PrintLog("NOTICE",$cont);
}
sub ERROR_LOG          
{                       
        my ($cont) = @_;
        PrintLog("ERROR",$cont);
} 
sub TrimStr
{
	my $str = shift;
	$str = $1 if($str =~ /^\s*(\S.+)$/);
	$str = $1 if($str =~ /^(.+?\S)\s*$/);
	return $str;
}
sub DEBUG_LOG  
{               
        my ($cont) = @_;
        PrintLog("DEBUG",$cont) if($LOG_LEVEL>=16);
}
sub FATAL_LOG
{
        my ($cont) = @_;
        PrintLog("FATAL",$cont);
}
sub WARN_LOG
{
        my ($cont) = @_;
        PrintLog("WARN",$cont);
}
sub GetSignSizeFile
{
	my $path = GetOutputPath();
        my $file = "$path/size-sign.file";
#	$file = "$path/size-sign.test";
	return $file;
}
sub GetUASignFile
{
	my $path = GetOutputPath();
	my $file = "$path/ua-accept-size.signfile";
}
sub GetProfileFile
{
	my $path = GetOutputPath();
        my $file = "$path/profile.file";
}
sub GetProfileDataPath
{
	my $path = GetOutputPath();
        my $file = "$path/profile-data/";
}
sub GetProfileMapFile
{
	my $path = GetProfileDataPath();
        my $file = "$path/profile-map.file";

}
sub GetMobileSizeFile
{
	my $path = GetOutputPath();
        my $file = "$path/mobile-ssize.txt";
}
sub ReadGroup
{
        my ($group,$fhandler) = @_;
        my $itemnum = 0;
        %$group = ();
        my $begin = 0;
        $group->{"itemnum"} = 0;
        my $readnum = 0;
	my $expectnum = 0;
	my $beginnum = 0;
        while(my $line = <$fhandler>)
        {
                chomp($line);
 #               print "read line:$line\n" ;#if($linenum == 0 && $readnum==0);
                $readnum++;
                if($line =~ /^\s*$/)
                {
			next;
                }
                if($line =~ /^GROUP:(\d+)\s+itemnum:(\d+)\s*$/)
                {
                        $begin = 1;
			$expectnum = $2;
                        next;
                }elsif($begin == 0)
                {
                        next;
                }
                if($line =~ /^\s*(\S+?)\s*:\s*(\S.+)$/)
                {
                        my $name = $1; my $value = $2;
                        $group->{"item$itemnum-name"} = $name;
                        $group->{"item$itemnum-value"} = $value;
                        $group->{"header-$name"} = $value;
                        $itemnum ++;
                }
		$beginnum++;
		last if($beginnum >= $expectnum);
        }
        $group->{"itemnum"} = $itemnum;
        return $itemnum;

}
sub GetSetKeyValue
{
	my ($set ) = @_;
	my $str = "";
	while(my ($k,$v) = each(%$set))
	{
		$str .= "\t" if($str =~ /\S/);
		$str .= "[$k]:[$v]";
	}
	return $str;
}
sub DumpSetToFile
{
	my ($set,$file) = @_;
	my $str = "";
	while(my ($name,$value) = each(%$set))
	{
		$str .= "$name\t$value\n";
	}
	SaveContentToFile($file,$str);
}
sub AddNumToSet
{
	my ($set,$name,$num) = @_;
	 $set->{$name} = 0 if(!exists($set->{$name}));
        $set->{$name} += $num;
        return $set->{$name};
}
sub AddToSet
{
	my ($set,$name) = @_;
	$set->{$name} = 0 if(!exists($set->{$name}));
	$set->{$name} ++;
	return $set->{$name};
}                                             
sub SetFlagFile
{
	my ($file) = @_;
	if( -e $file)
	{
		NOTICE_LOG("$SN SetFlagFile flag file exists:$file");
		return 0;
	}
	`echo "working">$file`;
	NOTICE_LOG("$SN SetFlagFile set file:$file");
	return 1;
}
sub CleanFlagFile
{
	my ($file) = @_;
	if( -e $file)
	{
		`rm -rf $file`;
	}
	NOTICE_LOG("$SN CleanFlagFile clean file:$file");
	return 1;
}

sub Trans_gbk2utf8
{
	my ($inputfile, $outputfile) = @_;
	if (!open (INFP, "$inputfile")) {
		ERROR_LOG("cann't open inputfile: $inputfile");
		return 0;
	}
	if (!open (OUTFP, ">$outputfile")) {
		ERROR_LOG("cann't open outputfile: $outputfile");
		return 0;
	}
	while (defined(my $line = <INFP>)) {
		my $utf8line = encode("utf8", decode("gbk",$line));
		$utf8line =~ s/[\x01-\x09\x0B-\x1F]//g;
		print OUTFP $utf8line;
	}
	close(OUTFP);
	close(INFP);

	MakeReleaseFileMd5($outputfile);
	if (!-e "$outputfile.md5") {
		ERROR_LOG("cann't makereleasefilemd5: $outputfile");
		return 0;
	}

	return 1;
}

sub MailMessageTo
{       
        my($tit,$cont,$body,$MAIL_TO) = @_;
        $cont .= "\n$body";
        my $dt = GetDate();
        $cont =~ s/"/\\"/g;
        my $cmd = "echo \"$cont\" |  mail -s \"$tit-($dt)\" $MAIL_TO";
#	NOTICE_LOG("send mail cmd:[$cmd]");
        `$cmd`;
		return 1;
}

sub ShotMessageTo
{
	my ($phonelist, $msg) = @_;
	my $host = `hostname`;
	my @phones = split(/\s+/, $phonelist);
	for my $phone (@phones) {
		my $cmd = "/bin/gsmsend-script $phone"."@"."$host:$msg";
		$cmd =~ s/[\r\n]//g;
#   NOTICE_LOG("send short message cmd:[$cmd]");
		system($cmd);
	}
}


1;


