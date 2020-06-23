<?php

// Переводит координаты в классические для шахмат и обратно
function CoordsToCellName($cell)
{
    $letter = chr(ord('A') + $cell['x']);
    $number = strval($cell['y'] + 1);
    return $letter . $number;
}

function CellNameToCoords($name)
{
    $name = strtoupper($name);
    return [
        "x" => ord($name[0]) - ord('A'),
        "y" => intval($name[1]) - 1
    ];
}

function console_log($data)
{
    echo '<script>';
    echo 'console.log(' . json_encode($data) . ')';
    echo '</script>';
}