<?php

if (array_key_exists('filename', $_POST)) {
    shell_exec("./a3 " . $_POST['filename']);
}

if (array_key_exists('fileToUpload', $_FILES)) {
    $target_dir = "files/";
    $target_file = $target_dir . basename($_FILES["fileToUpload"]["name"]);
    $uploadOk = 1;
    $imageFileType = pathinfo($target_file,PATHINFO_EXTENSION);
    if (move_uploaded_file($_FILES["fileToUpload"]["tmp_name"], $target_file)) {
            echo "The file ". basename($_FILES["fileToUpload"]["name"]). " has been uploaded. <br>";
    } else {
            echo "Sorry, there was an error uploading your file. <br>";
    }
}

$files = array_filter(glob("files/*"), function($name) {
    return pathinfo($name, PATHINFO_EXTENSION) != 'info';
});

$htmlfiles = array_filter($files, function($name) {
    return pathinfo($name, PATHINFO_EXTENSION) == 'html';
});

$textfiles = array_filter($files, function($name) {
    return pathinfo($name, PATHINFO_EXTENSION) != 'html';
});

echo <<<FORM
<button style='float:left;' id="myBtn">Convert</button>
<div id="convertModal" class="modal">
    <div class="modal-content">
        <span class="close">x</span>
        <div>
            <ul>
FORM;
              foreach ($files as $file):
                  echo "<li onclick='selectFilename(this);' style='cursor: pointer; display: inline-block; padding: 5px;'> $file </li>";
                  echo "<br>";
              endforeach;
echo <<<FORM
            </ul>
        </div>
        <p>
            <form action="a3.php" method="post">
                <input class="convert" type="text" hidden name="filename" value="">
                <input type="Submit" value="OK">
            </form>
        </p>
    </div>
</div>

<form style="float: left; padding-left: 5px;" id="upload" action="a3.php" method="post" enctype="multipart/form-data">
    <div style="width: 60px;">
        <label style='background-color: buttonface; padding: 2px; border-width: 1px; border-style: outset; border-color: #D3D3D3;' for="fileToUpload">Upload</label>
        <input id="fileToUpload" name="fileToUpload" style="visibility:hidden;" type="file">
    </div>
</form>
FORM;

echo <<<FORM
<button style='float:left;' id="myBtn">DB Store</button>
<div id="DBModal" class="modal">
    <div class="modal-content">
        <span class="close">x</span>
        <div>
            <ul>
FORM;
              foreach ($files as $file):
                  echo "<li onclick='selectFilename(this);' style='cursor: pointer; display: inline-block; padding: 5px;'> $file </li>";
                  echo "<br>";
              endforeach;
echo <<<FORM
            </ul>
        </div>
        <p>
            <form action="a3.php" method="post">
                <input class="convert" type="text" hidden name="filename" value="">
                <input type="Submit" value="OK">
            </form>
        </p>
    </div>
</div>
<div style="clear: both;"></div>
FORM;

echo "<div style='float: left; width: 50%;'><ul>";
foreach ($textfiles as $file) {
    echo "<li> <a href='$file'> $file</a> </li>";
}
echo "</ul></div>";

echo "<div style='float: left; width: 50%;'><ul>";
foreach ($htmlfiles as $file) {
    echo "<li> <a href='$file'> $file</a> </li>";
}
echo "</ul></div><div style='clear: both;'></div>";

print <<<JAVASCRIPT
    <script>
        document.getElementById("fileToUpload").onchange = function() {
            document.getElementById("upload").submit();
        };
    </script>
JAVASCRIPT;


echo <<< MODAL

<style>

.modal {
    display: none; /* Hidden by default */
    position: fixed; /* Stay in place */
    z-index: 1; /* Sit on top */
    left: 0;
    top: 0;
    width: 100%; /* Full width */
    height: 100%; /* Full height */
    overflow: auto; /* Enable scroll if needed */
    background-color: rgb(0,0,0); /* Fallback color */
    background-color: rgba(0,0,0,0.4); /* Black w/ opacity */
}

/* Modal Content/Box */
.modal-content {
    background-color: #fefefe;
    margin: 15% auto; /* 15% from the top and centered */
    padding: 20px;
    border: 1px solid #888;
    width: 80%; /* Could be more or less, depending on screen size */
    display: inline-block;
}

/* The Close Button */
.close {
    color: #aaa;
    float: right;
    font-size: 28px;
    font-weight: bold;
}

.close:hover,
.close:focus {
    color: black;
    text-decoration: none;
    cursor: pointer;
}

</style>

<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.1.1/jquery.min.js"></script>
<script>
    var modal = document.getElementById('convertModal');
    var modal = document.getElementById('DBModal');

    // Get the button that opens the modal
    var btn = document.getElementById("myBtn");

    // Get the <span> element that closes the modal
    var span = document.getElementsByClassName("close")[0];

    // When the user clicks on the button, open the modal
    btn.onclick = function() {
        modal.style.display = "block";
    }

    btn.onclick = function() {
        modal.style.display = "block";
    }

    function selectFilename(item) {
        $(item).parent().children().css({"background-color": "white"});
        $(item).css({"background-color": "#D3D3D3"});
        $(".convert").attr("value", $(item).text().trim());
    }

    // When the user clicks on <span> (x), close the modal
    span.onclick = function() {
        modal.style.display = "none";
    }

    // When the user clicks anywhere outside of the modal, close it
    window.onclick = function(event) {
        if (event.target == modal) {
            modal.style.display = "none";
        }
    }

</script>

MODAL;
