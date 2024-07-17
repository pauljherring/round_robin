<!DOCTYPE html>
<html>
<head>
<style>
details {
	font:
		16px "Open Sans",
		Calibri,
		sans-serif;
	width: 90%;
}

details > summary {
	margin-top: 5px;
	padding: 2px 6px;
	width: 90%;
	background-color: #ddd;
	border: none;
	box-shadow: 3px 3px 4px black;
	cursor: pointer;
}

details > pre {
	border-radius: 0 0 10px 10px;
	background-color: #ddd;
	padding: 2px 6px;
	box-shadow: 3px 3px 4px black;
}

details[open] > summary {
	background-color: #ccf;
}
pre {
	border-radius: 0 0 10px 10px;
	background-color: #edd;
	padding: 2px 6px;
	width: 90%;
}
form {
	width: 50%;
	box-shadow: 0 0 2px 2px blue;
}
</style>
</head>


<h1>Round Robin Generator</h1>

<p><a href="https://github.com/pauljherring/round_robin?tab=readme-ov-file#round-robin">Github repository</a> with source for a command line version of this tool.</p>

<h2>Example config files</h2>
<ul>
	<li><details><summary>Example with detailed comments</summary>
	<p>See <a href="https://github.com/pauljherring/round_robin?tab=readme-ov-file#input-file-format">Input File Format</a> on Github for more details.</p>
<pre>
# Lines starting with a hash are ignored

# Header

# Description of the division. Not currently used in anything but the human readable output
Division: &lt;name of division>

# When matches would normally start. Used in the League Republic output to populate that
Time: &lt;time for matches, default 20:00>

# List of teams
# The number on the left, before the colon, must be unique in this file, but
# numbers need not be sequential. It's suggested that you start at one and go from
# there
# &lt;nth team name> is a human readable name, which is output in that particular mode
# &lt;venue name> optional; the name of the venue if vastly different from the team name
# &lt;short code> is the `Short Code` for the team in League Republic and MUST be present before upload
1: &lt;first team name>{|&lt;venue name>}{%&lt;short code>}
2: &lt;second team name>{|&lt;venue name>}{%&lt;short code>}
3: &lt;...>
...

# When fixtures happen. Lines must start with `week` (case insensitive) followed by a
# monotonically incrementing number.
#
# If you wish two sets of fixtures on the same date, use that same date on two consecutive
# 'weeks'
#
# Hopefully obviously, if you want calendar weeks off, simply omit them and use WeekN+1
# for the next calendar match. For (real) example, 21/22 Christmas with our matches on Thursdays:
# Week14:09/12/2021
# Week15:16/12/2021
#  &lt;xmas/new year here>
# Week16:06/01/2022 # NOT week 18.
# Week17:20/01/2022
#
# Not monotonically incrementing the Week# number will result in fixture sets for those missing
# being omitted.
Week1:dd/mm/yyy
Week2:dd/mm/yyy
Week3:dd/mm/yyy
...
Week19:dd/mm/yyy
</pre></details></li>
<li><details><summary>Config used for 2023/24 <a href="https://www.facebook.com/groups/gatesheadpool">Gateshead and District Pool League</a></summary>
<pre>
Division:D1
# Time: <time for matches, default 20:00>
1:Birtley A%BRTA
2:Birtley B%BRTB
3:Coach & Horses A%CAHA
4:Corporation Club%COR
5:Eagle%EAG
6:Pelaw Inn%PEL
7:Mallard A%MALA
8:Mallard B%MALB
9:Travellers Rest A%TR
10:Travellers Rest B%PLOC
11:William IV%WIV
12:Sky Lounge A%SKY


#Division:D2
#1:Brandling A%BRNA
#2:Brandling B%BRNB
#3:Azure Blue%AZB
#4:Coach & Horses A%CAHA
#5:Gold Medal%GLDM
#6:Fiddlers%FIDD
#7:Guide Post%GUDP
#8:Nag's Head%NGH
#9:Plough%PLOA
#10:Saltwell Club%STL
#11:Sky Lounge B%SKYB
#12:Top Bay Horse%TBH


Week1:07/09/2023
Week2:14/09/2023
Week3:21/09/2023
Week4:28/09/2023
Week5:05/10/2023
Week6:19/10/2023
Week7:26/10/2023
Week8:09/11/2023
Week9:16/11/2023
Week10:30/11/2023
Week11:07/12/2023
Week12:14/12/2023
Week13:11/01/2024
Week14:25/01/2024
Week15:01/02/2024
Week16:15/02/2024
Week17:22/02/2024
Week18:07/03/2024
Week19:14/03/2024
Week20:28/03/2024
Week21:04/04/2024
Week22:18/04/2024
Week23:25/04/2024
Week24:02/05/2024
Week25:16/05/2024
Week26:23/05/2024
Week27:06/06/2024
Week28:13/06/2024
Week29:27/06/2024
Week30:04/07/2024
Week31:11/07/2024
Week32:18/07/2024
Week33:25/07/2024

</pre></details></li>
</ul>

<h2>Upload a config file</h2>
<form action="<?= $_SERVER['PHP_SELF']?>" method="post" enctype="multipart/form-data">
	<input type="file" id="myFile" name="filename"><br>
	<p>Output format:<br>
		<input type="radio" id="human" name="output_format" value="human" checked>
		<label for="human">Human Readable</label><br>
		<input type="radio" id="league" name="output_format" value="league">
		<label for="league">League Republic</label><br>
	</p>
	<input type="submit" value="Upload file">
</form>


<?php
$opts = '';
switch ($_POST['output_format']) {
	case 'human':
		$opts = "-m";
		break;
	case 'league':
		$opts = "-l";
		echo "Copy and save any output as <tt>&lt;filename>.csv</tt>, including the header line starting with <tt>Date dd/mm/yy</tt>:";
		break;
}

if (($fn = $_FILES['filename']['tmp_name'])) {
	$output = shell_exec("./generate_schedule $opts -f $fn");
	echo "<pre onclick='clip(this);' id='league_output'>$output</pre>";
}

?>
<script>
	var clip = function(el) {
	var range = document.createRange();
	range.selectNodeContents(el);
	var sel = window.getSelection();
	sel.removeAllRanges();
	sel.addRange(range);
};</script>
</html>