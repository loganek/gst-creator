#!/usr/bin/perl

# state:
#   0 - no enum found
#   1 - enum class declaration found
#   2 - `{` found
#   3 - `,` not found in enum value (last value in enum)

sub syntax_error($$) {
	my $line_number = shift;
	my $message     = shift;
	print "Syntax error on line " . $line_number . ": " . $message . "\n";
	exit;
}

sub state_error($$$) {
	my $line_number = shift;
	my $expected    = shift;
	my $current     = shift;

	my $message = "expected state `$expected`, but `$current` state found";

	syntax_error( $line_number, $message );
}

sub generate_converter {
	my ( $enum_name, @enum_values ) = @_;

	$ret = "template<>\n";
	$ret .= "std::string enum_to_string<$enum_name>($enum_name enum_value)\n";
	$ret .= "{\n";
	$ret .= "\tswitch (enum_value)\n";
	$ret .= "\t{\n";

	for ( my $i = 0 ; $i < @enum_values ; $i++ ) {
		$ret .= "\tcase $enum_name\::$enum_values[$i]:\n";
		$ret .= "\t\treturn \"$enum_values[$i]\";\n";
	}

	$ret .= "\tdefault:\n";
	$ret .= "\t\t throw std::runtime_error(\"unknow enum value\");\n";
	$ret .= "\t}\n";
	$ret .= "}\n\n";

	$ret .= "template<>\n";
	$ret .= "$enum_name string_to_enum(const std::string& enum_value)\n";
	$ret .= "{\n";
	$ret .= "\tstd::string val = enum_value;\n\n"; # TODO to upper
	
	for ( my $i = 0 ; $i < @enum_values ; $i++ ) {
		$ret .= "\tif (val == \"$enum_values[$i]\")\n";
		$ret .= "\t\treturn $enum_name\::$enum_values[$i];\n";
	}

	$ret .= "\n\tthrow std::runtime_error(\"cannot find enum value\");\n";
	$ret .= "}\n\n";
	
	return $ret;
}

sub trim($) {
	my $s = shift;
	$s =~ s/^\s+//;
	$s =~ s/\s+$//;
	return $s;
}

$arg_cnt = $#ARGV + 1;

if ( $arg_cnt != 2 ) {
	print
	  "\nUsage: enmu_string_generator.pl <enum's filename> <output filename>\n";
	exit;
}

my $autogenerate_string = "// auto-generated file! Don't modify it!\n\n";
my $state = 0;    # TODO check enum
my $enum_name;
my @enum_values = ();

open my $input_file, "<", $ARGV[0] or die $!;
open my $output_file, ">", $ARGV[1] or die $!;

$ifdef_str = uc($ARGV[1]);
$ifdef_str =~ s/[^a-zA-Z\d]+/_/g;

print $output_file $autogenerate_string;
print $output_file "#ifndef ".$ifdef_str."\n";
print $output_file "#define ".$ifdef_str."\n\n";
print $output_file "#include <stdexcept>\n\n";


if ($ARGV[0] =~ m/include\/(.*)$/) {
	$include_str = $1;
} else {
	$include_str = $ARGV[0];
}

print $output_file "#include \"$include_str\"\n\n";

print $output_file "template<typename T>\n";
print $output_file "T string_to_enum(const std::string&);\n";

print $output_file "template<typename T>\n";
print $output_file "std::string enum_to_string(T);\n\n";

while ( my $line = <$input_file> ) {
	$line = trim($line);
	if ( $line =~ m/enum\s*class\s*(.+)$/ ) {
		if ( $state != 0 ) {
			state_error( $., 0, $state );
		}
		$state     = 1;
		$enum_name = $1;
	}
	elsif ( $state == 1 && $line eq "{" ) {
		$state = 2;
	}
	elsif ( ( $state == 2 || $state == 3 ) && $line =~ m/}\s*;$/ ) {
		print $output_file generate_converter( $enum_name, @enum_values );

		@enum_values = ();
		$state       = 0;
	}
	elsif ( $state == 2 && $line =~ m/(.+)\s*=\s*(\d+)\s*(, *)$/ )
	{                        # TODO it accepts VALUE = 3,, - it's incorrect
		if ( $3 ne "," ) {
			print $1." Not equal\n";
			$state = 3;
		}

		push( $enum_values, $1 );
	}
	elsif ( $state == 2 && $line =~ m/(.+)\s*(, *)$/ ) {
		if ( $2 ne "," ) {
			print $2."Not equal\n";
			$state = 3;
		}

		push( @enum_values, $1 );
	}
}

print $output_file "\n#endif\n";