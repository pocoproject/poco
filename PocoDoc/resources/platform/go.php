<?php 
$category = "index-all.html";
$page = "welcome.html";
if (array_key_exists('c', $_GET))
{
	$category = 'category-' . $_GET['c'] . '-index.html';
}
if (array_key_exists('p', $_GET))
{
	$page = $_GET['p'] . '.html';
}
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html>
<head>
<title>Applied Informatics C++ Libraries and Tools Reference Library</title>
</head>
<frameset cols="25%,75%" title="">
<frameset rows="40%,60%" title="">
<frame src="overview.html" title="Overview" name="overviewFrame" />
<frame src="<?php echo $category; ?>" title="Index" name="indexFrame" />
</frameset>
<frame src="<?php echo $page; ?>" title="Details" name="detailsFrame" />
</frameset>
<noframes>
<h1>Frames Required</h1>
<p>This document is designed to be viewed using the frames feature. 
If you see this message, you are using a non-frame-capable web client.</p>
</noframes>
</html>
