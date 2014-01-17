package module_gen;

BEGIN {
    unshift @INC,"./";
}

use Funclib;
use strict;
use POSIX qw(strftime); 

my $SN = "module_gen";

sub parse_class
{
    my ($fh, $template_data, $class_name)= @_;

    my $is_first_key_member = 1;
    my %class_body = ();
    my $FSN = "parse_class";

    Funclib::ERROR_LOG("$FSN start[class_name:$class_name]");
    
    while (<$fh>) 
    {
        my ($line) = $_;
        
        chomp($line);
        $line =~ s/ //g;

        Funclib::ERROR_LOG("line:$line");
        
        if($line =~ /{/)
        {
            next;
        }
        elsif($line =~ /}/)
        {
            Funclib::ERROR_LOG("$FSN :set class [key:$class_name]");
            $template_data->{$class_name} = \%class_body;
            return 1;
        }
        else
        {
             my %column_info = ();

             if($line =~ /^(.*):([^[]*)\[\]/) 
             {           
                 $column_info{"type_name"}=$2;
                 $column_info{"type_tag"} = "vector";

                 if($is_first_key_member == 1)
                 {
                     $column_info{"key_member"} = 1;
                     $is_first_key_member = 0;
                 }
                 else
                 {
                     $column_info{"key_member"} = 0;
                 }

                 Funclib::ERROR_LOG("$FSN :[key:$1],[value:$2],[is_key:$column_info{'key_member'} ]. Is vector");
                 $class_body{$1}= \%column_info;
             }
             elsif($line =~ /^(.*):(.*)$/)
             {
                 $column_info{"type_name"}=$2;
                 $column_info{"type_tag"} = "single";

                 if($is_first_key_member == 1)
                 {
                     $column_info{"key_member"} = 1;
                     $is_first_key_member = 0;
                 }
                 else
                 {
                     $column_info{"key_member"} = 0;
                 }

                 Funclib::ERROR_LOG("$FSN :[key:$1],[value:$2],[is_key:$column_info{'key_member'} ]. Not vector");
                 $class_body{$1} = \%column_info;
             }
             elsif($line = /^\s+$/)
             {
                 Funclib::ERROR_LOG("$FSN : Empty Line");
                 next; 
             }
             else
             {
                 Funclib::ERROR_LOG("$FSN :Error format!!");
                 return -1;
             }
        }
    }
}

sub parse_template_file
{
    my ($tmpl_file, $template_data)= @_;
    my $FSN = "parse_template_file";

    Funclib::ERROR_LOG("$SN $FSN start open template_file:$tmpl_file");

    # open readonly template file 
    if(!open(*TMPLFP, "$tmpl_file")) 
    {
        Funclib::ERROR_LOG("$SN $FSN cann't open template_file:$tmpl_file");
        return -1;
    }

    my $fh = *TMPLFP;

    while (<$fh>) 
    {
        my ($line) = $_;

        chomp($line);
        Funclib::ERROR_LOG("line:$line");
         
        if($line =~ /^([^=]*)=(.*)$/) 
        {
            $line =~ s/ //g;
            $template_data->{$1} = $2;
            Funclib::ERROR_LOG("key:$1, value:$2");
        }
        elsif($line =~ /class\s+([^\s]*)/)
        {
            $1 =~ s/ //g;
            Funclib::ERROR_LOG("$FSN [class_name:$1]");
            my $ret = parse_class(\$fh, $template_data, $1);

            if($ret < 0)
            {
                Funclib::ERROR_LOG("$FSN : Class Parse Error!!");
                return -1;
            }
        }
        elsif($line = /^\s+$/)
        {
            Funclib::ERROR_LOG("$FSN : Empty Line");
            next;
        }
        else
        {
            Funclib::ERROR_LOG("$FSN : Parse Error. Not Support Format!!");
            return -1;
        }
    }

    close(TMPLFP);
    return 0;
}

sub generate_record_body
{
    my ($template_data, $body_context, $record_body)= @_;
    
    my $FSN = "generate_record_body";
    my $is_first_vector = 1;
    my $has_sub_class = 0;

    my @main_context;
    my @clear_context;

    push @clear_context, sprintf(" "x4 . "%s", 'void clear()');
    push @clear_context, sprintf(" "x4 . "%s", '{');
    push @clear_context, sprintf(" "x8 . "%s", 'LOGG("[GWJ] %s:  start ", __FUNCTION__);');

    foreach my $column_name (keys %$record_body) 
    {
        my $column_info = $record_body->{$column_name};

        if(!defined($column_info))
        {
            Funclib::ERROR_LOG("$FSN : Column[$column_name] Error!");
            return -1;
        }       

        my $type_name = $column_info->{"type_name"};
        my $type_tag = $column_info->{"type_tag"};

        if(!defined($type_name))
        {
            Funclib::ERROR_LOG("$FSN : Column[$column_name] type_name Error!");
            return -1;
        } 
        
        if(!defined($type_tag))
        {
            Funclib::ERROR_LOG("$FSN : Column[$column_name] type_tag Error!");
            return -1;
        }      

        Funclib::ERROR_LOG("$FSN : Column[type:$type_name];[col_name:$column_name];[tag:$type_tag]");

        my $sub_class_body = $template_data->{$type_name};
        if(defined($sub_class_body))
        {
            Funclib::ERROR_LOG("$FSN : Column name has sub_class[$type_name]");
            $has_sub_class = 1;
        }   
        else
        {
            Funclib::ERROR_LOG("$FSN : [$type_name] Atom Type");
            $has_sub_class = 0;
        }

        if($type_tag eq "single")
        {
            push @main_context, sprintf(" "x4 . "%s", "$type_name $column_name;");

            if($type_name eq "string")
            {
                push @clear_context, sprintf(" "x8 . "%s", "$column_name.clear();");
            }  
        }
        elsif($type_tag eq "vector")
        {
            if($has_sub_class == 1)
            {
                push @main_context, sprintf(" "x4 . "%s", "vector<MCPTupleItem> $column_name;");
            }
            else
            {
                push @main_context, sprintf(" "x4 . "%s", "vector<$type_name> $column_name;");
            }

            if($type_name eq "int" || $type_name eq "double")
            {
                Funclib::ERROR_LOG("$FSN : Integer/Double($type_name) Vector");
            }
            else
            {
                if($is_first_vector == 1)
                {
                    push @clear_context, "\n";
                    push @clear_context, sprintf(" "x8 . "%s", "int size = $column_name.size();");
                    $is_first_vector = 0;
                }
                else
                {
                    push @clear_context, "\n";
                    push @clear_context, sprintf(" "x8 . "%s", "size = $column_name.size();");
                }
            
                push @clear_context, sprintf(" "x8 . "%s", 'for(int i = 0; i < size; i++)');
                push @clear_context, sprintf(" "x8 . "%s", '{');

                if($has_sub_class == 1)
                {
                    push @clear_context, sprintf(" "x12 . "%s", "$column_name\[i\].key.clear();");
                    push @clear_context, sprintf(" "x12 . "%s", "$column_name\[i\].value.clear();");
                }
                else
                {
                    push @clear_context, sprintf(" "x12 . "%s", "$column_name\[i\].clear();");
                }

                push @clear_context, sprintf(" "x8 . "%s", '}');
            }
            
            push @clear_context, sprintf(" "x8 . "%s", "$column_name.clear();\n");
        }
        
    }

    push @clear_context, sprintf(" "x8 . "%s", 'LOGG("[GWJ] %s:  end ", __FUNCTION__);');
    push @clear_context, sprintf(" "x4 . "%s", '}');
    push @main_context, "\n";

    my $main_string = join "\n", @main_context;
    my $clear_string = join "\n", @clear_context;
    
    $$body_context .= $main_string;
    $$body_context .= $clear_string;
    return 1;
}

sub generate_header_file
{
    my ($template_data)= @_;
    
    my $FSN = "generate_header_file";
    my $coll_name = $template_data->{"MONGODB_COLL_NAME"};
    Funclib::ERROR_LOG("$FSN : Collection Name:$coll_name");
    
    my $module_name = $template_data->{"MODULE_CLASS_NAME"};
    Funclib::ERROR_LOG("$FSN : Module Name:$module_name");
    
    my $record_name = $template_data->{"RECORD_CLASS_NAME"};
    Funclib::ERROR_LOG("$FSN : Record Name:$record_name");
    
    my $record_class_body = $template_data->{$record_name};
    my $header_file_name = "$coll_name.h";

    my @header_strings;
    my @header_strings_2;
    my $header_file_context;
    my $ret = 1;
    
    if(!defined($coll_name))
    {
        Funclib::ERROR_LOG("$FSN : Collection Name Error!");
        return -1;
    }

    if(!defined($module_name))
    {
        Funclib::ERROR_LOG("$FSN : Module Name Error!");
        return -1;
    }

    if(!defined($record_name))
    {
        Funclib::ERROR_LOG("$FSN : Record Name Error!");
        return -1;
    }

    if(!open(HFILEFP, ">$header_file_name")) 
    {
        Funclib::ERROR_LOG("$SN $FSN cann't open $header_file_name");
        return -1;
    }
    
    Funclib::ERROR_LOG("$FSN [coll_name:$coll_name];[module:$module_name];[record:$record_name];[h_file:$header_file_name]");

    push @header_strings, "#ifndef $coll_name".'_H_';
    push @header_strings, "#define $coll_name".'_H_';
    push @header_strings, "\n";
    push @header_strings, '#include "abstract_mcp_content.h"';
    push @header_strings, "\n";
    push @header_strings, "class $record_name : public AbstractMCPRecord";
    push @header_strings, '{';
    push @header_strings, 'public:';
    push @header_strings, "\n";
    push @header_strings, sprintf(" "x4 . "%s", 'static const char* ClassName;');
    push @header_strings, "\n";

    $header_file_context = join "\n", @header_strings;
    $ret = generate_record_body($template_data, \$header_file_context, $record_class_body);

    if($ret < 0)
    {
        Funclib::ERROR_LOG("$FSN generate_record_body Error!!");
        return -1;
    }

    push @header_strings_2, "\n";
    push @header_strings_2, sprintf(" "x4 . "%s", "$record_name():AbstractMCPRecord()");
    push @header_strings_2, sprintf(" "x4 . "%s", '{');
    push @header_strings_2, sprintf(" "x4 . "%s", '}');
    push @header_strings_2, "\n";
    push @header_strings_2, sprintf(" "x4 . "%s", "virtual ~$record_name()");
    push @header_strings_2, sprintf(" "x4 . "%s", '{');
    push @header_strings_2, sprintf(" "x8 . "%s", 'this->clear();');
    push @header_strings_2, sprintf(" "x4 . "%s", '}');
    push @header_strings_2, '};';
    push @header_strings_2, "\n";
    push @header_strings_2, "\n";
    push @header_strings_2, "class $module_name : public AbstractMCPContent";
    push @header_strings_2, '{';
    push @header_strings_2, 'protected:';
    push @header_strings_2, sprintf(" "x4 . "%s", 'int set_basic_info_from_bson_result(bson* bson_out, const char* aggregate_name);');
    push @header_strings_2, "\n";
    push @header_strings_2, 'public:';
    push @header_strings_2, sprintf(" "x4 . "%s", "$module_name(const char* key_name);");
    push @header_strings_2, sprintf(" "x4 . "%s", "virtual ~$module_name();");
    push @header_strings_2, '};';
    push @header_strings_2, "\n";
    push @header_strings_2, "#endif /* $coll_name".'_H_ */';

    my $header_2 = join "\n", @header_strings_2;

    $header_file_context .= $header_2;
    print HFILEFP $header_file_context;
    close(HFILEFP);

    return 0;
}

sub generate_filter_context
{
    my ($template_data, $body_context, $record_body, $record_name)= @_;

    my $FSN = "generate_header_file";
    my $is_first_branch = 1;
    my $has_sub_class = 0;

    my @main_context;

    foreach my $column_name (keys %$record_body) 
    {
        my $column_info = $record_body->{$column_name};
    
        if(!defined($column_info))
        {
            Funclib::ERROR_LOG("$FSN : Column[$column_name] Error!");
            return -1;
        }       

        my $type_name = $column_info->{"type_name"};
        my $type_tag = $column_info->{"type_tag"};

        if(!defined($type_name))
        {
            Funclib::ERROR_LOG("$FSN : Column[$column_name] type_name Error!");
            return -1;
        } 
        
        if(!defined($type_tag))
        {
            Funclib::ERROR_LOG("$FSN : Column[$column_name] type_tag Error!");
            return -1;
        }      

        Funclib::ERROR_LOG("$FSN : Column[type:$type_name];[col_name:$column_name];[tag:$type_tag]");

        my $sub_class_body = $template_data->{$type_name};
        if(defined($sub_class_body))
        {
            Funclib::ERROR_LOG("$FSN : Column name has sub_class[$type_name]");
            $has_sub_class = 1;
        }   
        else
        {
            Funclib::ERROR_LOG("$FSN : [$type_name] Atom Type");
            $has_sub_class = 0;
        }

        if($is_first_branch == 1)
        {
            $is_first_branch = 0;
            push @main_context, sprintf(" "x8 . "%s", "if(0 == strcmp(key, \"$column_name\"))");
        }
        else
        {
            push @main_context, sprintf(" "x8 . "%s", "else if(0 == strcmp(key, \"$column_name\"))");
        }
        
        push @main_context, sprintf(" "x8 . "%s", '{');

        if($type_tag eq "vector")
        {
            if($has_sub_class == 1)
            {
                my @sub_names  = keys %$sub_class_body;
                my $not_key_index;
                my $key_index;

                if($sub_class_body->{$sub_names[0]}->{"key_member"} == 1)
                {
                    $not_key_index = 1;
                    $key_index = 0;
                }
                else
                {
                    $not_key_index = 0;
                    $key_index = 1;
                }

                my $value_type = $sub_class_body->{$sub_names[$not_key_index]}->{"type_name"};
                if($value_type eq "string")
                {
                    Funclib::ERROR_LOG("$FSN : [Column:$column_name];[tag:$type_tag];[type:$type_name]; SubClass[$sub_names[$not_key_index]],v_type[$value_type:$not_key_index] is String Tuple");
                    push @main_context, sprintf(" "x12 . "%s", "this->set_tuple_data_from_bson(it,");
                }
                elsif($value_type eq "int")
                {
                    Funclib::ERROR_LOG("$FSN : [Column:$column_name];[tag:$type_tag];[type:$type_name]; SubClass[$sub_names[$not_key_index]],v_type[$value_type:$not_key_index] is Integer Tuple");
                    push @main_context, sprintf(" "x12 . "%s", "this->set_tuple_data_from_bson_integer_value(it,");
                }
                else
                {
                    Funclib::ERROR_LOG("$FSN : [Column:$column_name];[tag:$type_tag];[type:$type_name]; SubClass[$sub_names[$not_key_index]],v_type[$value_type:$not_key_index] is Double Tuple");
                    push @main_context, sprintf(" "x12 . "%s", "this->set_tuple_data_from_bson_double_value(it,");
                }

                push @main_context, sprintf(" "x43 . "%s", "(void*)&detail->$column_name,");
                push @main_context, sprintf(" "x43 . "%s", 'true,');
                push @main_context, sprintf(" "x43 . "%s", "\"$sub_names[$key_index]\",         // KEY");
                push @main_context, sprintf(" "x43 . "%s", "\"$sub_names[$not_key_index]\");   // VALUE");

                push @main_context, "\n";
                push @main_context, sprintf(" "x12 . "%s", "LOGG(\"[GWJ] %s: set $record_name.$column_name\[size:%hu\]\",");
                push @main_context, sprintf(" "x21 . "%s", "__FUNCTION__, detail->$column_name.size());");

                Funclib::ERROR_LOG("$FSN : [Column:$column_name];[tag:$type_tag];[type:$type_name];Sub[$sub_names[0]:$sub_names[1]] OK");
            }
            else
            {
                push @main_context, sprintf(" "x12 . "%s", "this->set_vector_data_from_bson(it, detail->$column_name);");
                push @main_context, "\n";
                push @main_context, sprintf(" "x12 . "%s", "LOGG(\"[GWJ] %s: set $record_name.$column_name\[size:%hu\]\",");
                push @main_context, sprintf(" "x21 . "%s", "__FUNCTION__, detail->$column_name.size());");

                Funclib::ERROR_LOG("$FSN : [Column:$column_name];[tag:$type_tag];[type:$type_name] OK");
            }
        }
        elsif($type_tag eq "single")
        {
            if($type_name eq "string")
            {
                push @main_context, sprintf(" "x12 . "%s", "detail->$column_name = bson_iterator_string(it);");
                push @main_context, "\n";
                push @main_context, sprintf(" "x12 . "%s", "LOGG(\"[GWJ] %s: set $record_name.$column_name\[%s\]\",");
                push @main_context, sprintf(" "x21 . "%s", "__FUNCTION__, detail->$column_name.c_str());");
                
                Funclib::ERROR_LOG("$FSN : [Column:$column_name];[tag:$type_tag];[type:$type_name] OK");
            }
            elsif($type_name eq "int")
            {
                push @main_context, sprintf(" "x12 . "%s", 'ss.clear();');
                push @main_context, sprintf(" "x12 . "%s", 'ss << bson_iterator_int(it);');
                push @main_context, sprintf(" "x12 . "%s", "ss >> detail->$column_name;");
                push @main_context, "\n";
                push @main_context, sprintf(" "x12 . "%s", "LOGG(\"[GWJ] %s: set $record_name.$column_name\[%s\]\",");
                push @main_context, sprintf(" "x21 . "%s", "__FUNCTION__, detail->$column_name.c_str());");

                Funclib::ERROR_LOG("$FSN : [Column:$column_name];[tag:$type_tag];[type:$type_name] OK");
            }
            elsif($type_name eq "double")
            {
                push @main_context, sprintf(" "x12 . "%s", 'ss.clear();');
                push @main_context, sprintf(" "x12 . "%s", 'ss << bson_iterator_double(it);');
                push @main_context, sprintf(" "x12 . "%s", "ss >> detail->$column_name;");
                push @main_context, "\n";
                push @main_context, sprintf(" "x12 . "%s", "LOGG(\"[GWJ] %s: set $record_name.$column_name\[%s\]\",");
                push @main_context, sprintf(" "x21 . "%s", "__FUNCTION__, detail->$column_name.c_str());");

                Funclib::ERROR_LOG("$FSN : [Column:$column_name];[tag:$type_tag];[type:$type_name] OK");                
            }
            else
            {
                Funclib::ERROR_LOG("$FSN : [Column:$column_name];[tag:$type_tag];[type:$type_name] Not Support Error!!");
                return -1;
            }
        }
        else
        {
            
        }

        push @main_context, sprintf(" "x8 . "%s", '}');
    }

    my $main_string = join "\n", @main_context;
    $$body_context .= $main_string;

    return 1;
}

sub generate_key_index_context
{
    my ($body_context, $record_body)= @_;
    
    my $FSN = "generate_key_index_context";
    my $is_first_column = 1;
    my @main_context;
    
    foreach my $column_name (keys %$record_body) 
    {

         my $column_info = $record_body->{$column_name};
         my $type_name = $column_info->{"type_name"};
         my $type_tag = $column_info->{"type_tag"};
    
         if($is_first_column == 1)
         {
             if($type_tag eq "single")
             {
                 if($type_name eq "string" || $type_name eq "int" || $type_name eq "double")
                 {
                     $is_first_column = 0;
                     push @main_context, sprintf(" "x4 . "%s", "if(IS_SAME_VALUED_STRING(this->key_name, \"$column_name\"))");
                     push @main_context, sprintf(" "x4 . "%s", '{');
                     push @main_context, sprintf(" "x8 . "%s", "FILL_RESULT_BUCKET(detail, $column_name)");
                     push @main_context, sprintf(" "x4 . "%s", '}');
                     Funclib::ERROR_LOG("$FSN : Add first KEY-Index Branch[$column_name], Single basic column-type");
                 }
             }
         }
         else
         {
             if($type_tag eq "single")
             {
                 if($type_name eq "string" || $type_name eq "int" || $type_name eq "double")
                 {
                     push @main_context, sprintf(" "x4 . "%s", "else if(IS_SAME_VALUED_STRING(this->key_name, \"$column_name\"))");
                     push @main_context, sprintf(" "x4 . "%s", '{');
                     push @main_context, sprintf(" "x8 . "%s", "FILL_RESULT_BUCKET(detail, $column_name)");
                     push @main_context, sprintf(" "x4 . "%s", '}');                     
                     Funclib::ERROR_LOG("$FSN : Add later KEY-Index Branch[$column_name], Single basic column-type");
                 }
             }
         }
    }

    push @main_context, sprintf(" "x4 . "%s", 'LOGD("[GWJ] %s: end", __FUNCTION__);');
    push @main_context, sprintf(" "x4 . "%s", 'return 0;');
    push @main_context, '}';

    my $main_string = join "\n", @main_context;
    $$body_context .= $main_string;
    
    return 1;
}

sub generate_cpp_file
{

    my ($template_data)= @_;
    
    my $FSN = "generate_cpp_file";
    my $db_name = $template_data->{"MONGODB_DB_NAME"};
    my $coll_name = $template_data->{"MONGODB_COLL_NAME"};
    my $module_name = $template_data->{"MODULE_CLASS_NAME"};
    my $record_name = $template_data->{"RECORD_CLASS_NAME"};
    my $record_class_body = $template_data->{$record_name};
    my $cpp_file_name = "$coll_name.cpp";

    my $ret = 1;

    if(!defined($db_name))
    {
        Funclib::ERROR_LOG("$FSN : DB Name Error!");
        return -1;
    }

    if(!defined($coll_name))
    {
        Funclib::ERROR_LOG("$FSN : Collection Name Error!");
        return -1;
    }

    if(!defined($module_name))
    {
        Funclib::ERROR_LOG("$FSN : Module Name Error!");
        return -1;
    }

    if(!defined($record_name))
    {
        Funclib::ERROR_LOG("$FSN : Record Name Error!");
        return -1;
    }

    if(!defined($record_class_body))
    {
        Funclib::ERROR_LOG("$FSN : Record Body Error!");
        return -1;
    }

    if(!open(CPPFILEFP, ">$cpp_file_name")) 
    {
        Funclib::ERROR_LOG("$SN $FSN cann't open $cpp_file_name");
        return -1;
    }
    
    Funclib::ERROR_LOG("$FSN [coll_name:$coll_name];[module:$module_name];[record:$record_name];[cpp_file:$cpp_file_name]");
    
    my $cpp_file_context  = "#include \"$coll_name.h\"\n";
         $cpp_file_context .="\n";
         $cpp_file_context .="const char* $record_name\::ClassName = \"$record_name\";\n";
         $cpp_file_context .="\n";
         $cpp_file_context .="$module_name\::$module_name(const char* key_name):AbstractMCPContent(key_name)\n";
         $cpp_file_context .="{\n";
         $cpp_file_context .="    LOGD(\"[GWJ] %s: start\", __FUNCTION__);\n";
         $cpp_file_context .="\n";
         $cpp_file_context .="    this->_basic_coll = \"$coll_name\";\n";
         $cpp_file_context .="    this->_basic_ns = string(\"$db_name\") + \".\" + this->_basic_coll;\n";
         $cpp_file_context .="\n";
         $cpp_file_context .="    LOGD(\"[GWJ] %s: end\", __FUNCTION__);\n";
         $cpp_file_context .="\n";
         $cpp_file_context .="}\n";
         $cpp_file_context .="\n";
         $cpp_file_context .="$module_name\::~$module_name()\n";
         $cpp_file_context .="{\n";
         $cpp_file_context .="\n";
         $cpp_file_context .="    LOGD(\"[GWJ] %s: start. recycle result_set\", __FUNCTION__);\n";
         $cpp_file_context .="    RECYCLE_RESULT_SET($record_name)\n";
         $cpp_file_context .="    LOGD(\"[GWJ] %s: end\", __FUNCTION__);\n";
         $cpp_file_context .="\n";
         $cpp_file_context .="}\n";
         $cpp_file_context .="\n";
         $cpp_file_context .="int $module_name\::set_basic_info_from_bson_result(bson* bson_out, const char* aggregate_name)\n";
         $cpp_file_context .="{\n";
         $cpp_file_context .="\n";
         $cpp_file_context .="    LOGD(\"[GWJ] %s:  start\", __FUNCTION__);\n";
         $cpp_file_context .="\n";
         $cpp_file_context .="    const char* key;\n";
         $cpp_file_context .="    bson_iterator it[1];\n";
         $cpp_file_context .="    stringstream ss;\n";
         $cpp_file_context .="\n";
         $cpp_file_context .="    $record_name* detail = ObjectPoolFactory<$record_name>::instance().fetch_object();\n";
         $cpp_file_context .="    bson_iterator_init(it, bson_out);\n";
         $cpp_file_context .="\n";
         $cpp_file_context .="    while(bson_iterator_next(it))\n";
         $cpp_file_context .="    {\n";
         $cpp_file_context .="        ss.clear();\n";
         $cpp_file_context .="        key = bson_iterator_key(it);\n";
         $cpp_file_context .="\n";
         
         $ret = generate_filter_context($template_data, \$cpp_file_context, $record_class_body, $record_name);

         if($ret < 0)
         {
            Funclib::ERROR_LOG("$FSN generate_filter_context Error!!");
            return -1;
         }

         $cpp_file_context .="\n";
         $cpp_file_context .="    }// while\n";
         $cpp_file_context .="\n";

    generate_key_index_context(\$cpp_file_context, $record_class_body);

    print CPPFILEFP $cpp_file_context;
    close(CPPFILEFP);

    return 0;
}


