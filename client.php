<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
       "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<title>Socket Client</title>

<?PHP
$host_port = "5001";
$addr = gethostbyname("127.0.0.1");

// Getting the string we want to pass to the server
if (isset($_POST['Submit1'])) {
	$colorvalue = $_POST['inputv'];

	// Opening a socket connection to server
	$client = stream_socket_client("tcp://$addr:$host_port", $errno, $errorMessage);
	if ($client === false) {
    		throw new UnexpectedValueException("Failed to connect: $errorMessage");
	}

	// Send data stream to server
	fwrite($client, $colorvalue);
	
	// Receive server response
	echo stream_get_contents($client);
	// close socket connection
	fclose($client);
} // if
?>

</head>
<body>


<FORM NAME = userinput Method = "POST" Action = "testphp.php">

Input Value: <INPUT TYPE = Text NAME  = inputv SIZE = 8 value ="#">
<P>
<INPUT TYPE = Submit Name = Submit1 VALUE = "Set new value">
<P>

</FORM>
</body>

<html>
