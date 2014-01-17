#!/usr/bin/perl -w

use module_gen;

my $SN = "module_gen.pl";
my %template_data = ();
my $template_file_path = "../mongodb_template.tmpl";
my $ret = 0;

$ret = module_gen::parse_template_file($template_file_path, \%template_data);

if($ret < 0)
{
}
else
{
    $ret = module_gen::generate_header_file(\%template_data);

    if($ret < 0)
    {
    }
    else
    {
       $ret = module_gen::generate_cpp_file(\%template_data);
    }
}













