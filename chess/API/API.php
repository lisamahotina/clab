<?php
require_once $_SERVER['DOCUMENT_ROOT'] . "/Desk.php";
require_once $_SERVER['DOCUMENT_ROOT'] . "/Piece.php";


class API
{
    public function __construct()
    {
        header("Content-Type: application/json");

        session_start();

        if (!isset($_SESSION["desk"])) {
            $_SESSION["desk"] = new Desk();
        }
    }

    public function NewGame()
    {
        $_SESSION["desk"] = new Desk();
        return json_encode($_SESSION["desk"]);
    }

    public function GetState()
    {
        return json_encode($_SESSION["desk"]);
    }

    public function Move()
    {
        $from = CellNameToCoords($_GET["from"]);
        $to = CellNameToCoords($_GET["to"]);

        $piece = $_SESSION["desk"]->cells[$from["y"]][$from["x"]];

        if ($piece === null || $piece->isWhite !== $_SESSION["desk"]->isWhiteNext) {
            return json_encode([
                "moved" => false,
                "state" => $_SESSION["desk"]
            ]);
        } else {
            $moved = $piece->Move($to);
        }
        if ($moved) {
            $_SESSION["desk"]->isWhiteNext = !$_SESSION["desk"]->isWhiteNext;
        }

        // TODO Сделать проверку, что мы поставили мат
        return json_encode([
            "moved" => $moved,
            "state" => $_SESSION["desk"],
            "isCheck" => $piece->IsCheck(!$piece->isWhite)
        ]);
    }
}