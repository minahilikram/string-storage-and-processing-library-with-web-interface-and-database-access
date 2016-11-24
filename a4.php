<?php
$output = shell_exec("./getfiles ");
$DBfiles = preg_split('@(?=files/)@', $output);
array_shift($DBfiles);

if (isset($_GET["filename"])) {
    echo (shell_exec("./getfiles " . $_GET["filename"]));

echo <<<JS
    <script>
        $( document ).ready(function() {
            $(".main-page").hide();
        });
    </script>
JS;
}

echo "<div class='main-page' style='display: none;'>";

if (array_key_exists('filename', $_POST)) {
    shell_exec("./a4 " . $_POST['filename']);
}

if (array_key_exists('db', $_POST)) {
    shell_exec("./a4 " . $_POST['db'] . " store");
    header("Refresh:0");
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

$files = array_merge($files, $DBfiles);
sort($files);

echo <<<FORM
<button style='float:left;' id="convertModalBtn">Convert</button>
<div id="convertModal" class="convertModal">
    <div class="modal-content">
        <span class="close">x</span>
          <ul>
FORM;
            foreach ($files as $file):
                echo "<li onclick='selectFilename(this);' style='cursor: pointer; display: inline-block; padding: 5px;'> $file </li>";
                echo "<br>";
            endforeach;
echo <<<FORM
          </ul>
          <form action="a4.php" method="post">
              <input type="text" hidden name="filename" value="">
              <input type="Submit" value="OK">
          </form>
    </div>
</div>

<form style="float: left; padding-left: 5px;" id="upload" action="a4.php" method="post" enctype="multipart/form-data">
    <div style="width: 60px;">
        <label style='background-color: buttonface; padding: 2px; border-width: 1px; border-style: outset; border-color: #D3D3D3;' for="fileToUpload">Upload</label>
        <input id="fileToUpload" name="fileToUpload" style="visibility:hidden;" type="file">
    </div>
</form>
FORM;

echo <<<FORM
<button style='float:left;' id="DBModalBtn">DB Store</button>
<div id="DBModal" class="DBModal">
    <div class="modal-content">
        <span class="close">x</span>
          <ul>
FORM;
            foreach ($files as $file):
                echo "<li onclick='selectFilename(this);' style='cursor: pointer; display: inline-block; padding: 5px;'> $file </li>";
                echo "<br>";
            endforeach;
echo <<<FORM
          </ul>
          <form action="a4.php" method="post">
              <input type="text" hidden name="db" value="">
              <input type="Submit" value="OK">
          </form>
    </div>
</div>
<div style="clear: both;"></div>
FORM;

echo "<div style='float: left; width: 50%;'><ul>";
foreach ($files as $file) {
    if (in_array($file, $DBfiles)) {
        $path = $_SERVER['REQUEST_URI'];
        echo "<li> <a href='$path?filename=$file'> $file</a> </li>";
    }
    else {
        echo "<li> <a href='$file'> $file</a> </li>";
    }
}
echo "</ul></div><div style='clear: both;'></div>";
echo "</div>";

echo <<<HELPER

    <style>

        .DBModal, .convertModal {
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

        .close:hover, .close:focus {
            color: black;
            text-decoration: none;
            cursor: pointer;
        }

    </style>

    <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.1.1/jquery.min.js"></script>
    <script>
        var convertModal = document.getElementById('convertModal');
        var DBModal = document.getElementById('DBModal');

        // Get the button that opens the modal
        var convertModalBtn = document.getElementById("convertModalBtn");
        var DBModalBtn = document.getElementById("DBModalBtn");

        // Get the <span> element that closes the modal
        var convertSpan = $('.convertModal span')[0];

        var DBSpan = $('.DBModal span')[0];

        // When the user clicks on the button, open the modal
        convertModalBtn.onclick = function() {
            convertModal.style.display = "block";
        }

        DBModalBtn.onclick = function() {
            DBModal.style.display = "block";
        }

        // When the user clicks on <span> (x), close the modal
        convertSpan.onclick = function() {
            convertModal.style.display = "none";
        }

        DBSpan.onclick = function() {
            DBModal.style.display = "none";
        }

        // When the user clicks anywhere outside of the modal, close it
        window.onclick = function(event) {
            if (event.target == convertModal) {
                convertModal.style.display = "none";
            }

            if (event.target == DBModal) {
                DBModal.style.display = "none";
            }
        }

        function selectFilename(item) {
            $(item).parent().children().css({"background-color": "white"});
            $(item).css({"background-color": "#D3D3D3"});
            $(item).parent().parent().find('input:first').attr("value", $(item).text().trim());
        }

        document.getElementById("fileToUpload").onchange = function() {
            document.getElementById("upload").submit();
        };

        $( document ).ready(function() {
          if (window.location.href.indexOf('?') > -1) {
              $(".main-page").hide();
          }
          else {
              $(".main-page").show();
          }
        });

    </script>

HELPER;
