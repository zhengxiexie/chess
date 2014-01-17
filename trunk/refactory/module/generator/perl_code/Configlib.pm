package Configlib;

use strict;
use Funclib;

my $SN = "Configlib";
my $CONFFILE = "/home/work/noveldata/noveldataframe/conf/noveldataframe.conf";
my %gconf = ();
my %mysql = ();
my %portal = ();
my %dir = ();
my %chapter = ();


sub LoadConf
{
    if ((exists $gconf{"isok"}) && ($gconf{"isok"} != 0)) {
        return (1, \%gconf);
    }

    %gconf = Funclib::LoadConfFile($CONFFILE);
    if ($gconf{"isok"} == 0)
    {
        Funclib::ERROR_LOG("$SN LoadConf read global conf fail! file:$CONFFILE");
        return (0, \%gconf);
    }

    return (1, \%gconf);
}


sub LoadMysqlConf
{
    if ((exists $mysql{"isok"}) && ($mysql{"isok"} != 0)) {
        return \%mysql;
    }

    my $conffile = $gconf{"NOVELDATAFRAME_PATH"}.$gconf{"MYSQL_CONF"};
    %mysql = Funclib::LoadConfFile($conffile);
    if ($mysql{"isok"} == 0) {
        Funclib::ERROR_LOG("$SN LoadConf read global conf fail! file:$conffile");
    }

    return \%mysql;
}


sub LoadMysqlDay
{
    my $conffile = $gconf{"NOVELDATAFRAME_PATH"}.$gconf{"MYSQL_DAY"};
    my $mysqlday = `cat $conffile`;
    $mysqlday =~ s/[\r\n]//g;

    return $mysqlday;
}


sub LoadPortalConf
{
    if ((exists $portal{"isok"}) && ($portal{"isok"} != 0)) 
    {
        return \%portal;
    }

    my $conffile = $gconf{"NOVELDATAPARSE_PATH"}.$gconf{"SITE_PORTAL_PATTERN_FILE"};
    %portal = Funclib::LoadConfFile($conffile);
    if ($portal{"isok"} == 0) {
        Funclib::ERROR_LOG("$SN LoadConf read global conf fail! file:$conffile");
    }

    return \%portal;
}


sub LoadDirConf
{
    if ((exists $dir{"isok"}) && ($dir{"isok"} != 0)) 
    {
        return \%dir;
    }

    my $conffile = $gconf{"NOVELDATAPARSE_PATH"}.$gconf{"SITE_DIR_PATTERN_FILE"};
    %dir = Funclib::LoadConfFile($conffile);
    
    if ($dir{"isok"} == 0) 
    {
        Funclib::ERROR_LOG("$SN LoadConf read global conf fail! file:$conffile");
    }

    return \%dir;
}


sub LoadChapterConf
{
    if ((exists $chapter{"isok"}) && ($chapter{"isok"} != 0)) {
        return \%chapter;
    }

    my $conffile = $gconf{"NOVELDATAPARSE_PATH"}.$gconf{"SITE_CHAPTER_PATTERN_FILE"};
    %chapter = Funclib::LoadConfFile($conffile);
    if ($chapter{"isok"} == 0) {
        Funclib::ERROR_LOG("$SN LoadConf read global conf fail! file:$conffile");
    }

    return \%chapter;
}



1;
