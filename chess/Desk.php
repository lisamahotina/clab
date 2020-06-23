<?php
require_once $_SERVER['DOCUMENT_ROOT'] . "/Piece.php";


class Desk implements JsonSerializable
{
    const OUT_OF_DESK = -2;
    const ENEMY = -1;
    const EMPTY_CELL = 0;
    const FRIEND = 1;
    const ME = 2;

    const CELL_WIDTH = 8;
    const CELL_HEIGHT = 8;

    public $cells;
    public $isWhiteNext;

    function __construct()
    {
        $this->initializeCells();
        $this->initializePieces();
        $this->isWhiteNext = true;
    }

    function initializeCells()
    {
        $this->cells = [];
        for ($i = 0; $i < self::CELL_HEIGHT; $i++) {
            $row = [];
            for ($j = 0; $j < self::CELL_WIDTH; $j++) {
                array_push($row, null);
            }
            array_push($this->cells, $row);
        }
    }

    function initializePieces()
    {
        // White Pieces
        $wLeftRook = new Rook($this, true);
        $wLeftKnight = new Knight($this, true);
        $wLeftBishop = new Bishop($this, true);
        $wKing = new King($this, true);
        $wQueen = new Queen($this, true);
        $wRightRook = new Rook($this, true);
        $wRightKnight = new Knight($this, true);
        $wRightBishop = new Bishop($this, true);
        $this->cells[7][0] = $wLeftRook;
        $this->cells[7][1] = $wLeftKnight;
        $this->cells[7][2] = $wLeftBishop;
        $this->cells[7][3] = $wQueen;
        $this->cells[7][4] = $wKing;
        $this->cells[7][5] = $wRightBishop;
        $this->cells[7][6] = $wRightKnight;
        $this->cells[7][7] = $wRightRook;
        for ($i = 0; $i < self::CELL_WIDTH; $i++) {
            $this->cells[6][$i] = new Pawn($this, true);
        }
        // Black Pieces
        $bLeftRook = new Rook($this, false);
        $bLeftKnight = new Knight($this, false);
        $bLeftBishop = new Bishop($this, false);
        $bKing = new King($this, false);
        $bQueen = new Queen($this, false);
        $bRightRook = new Rook($this, false);
        $bRightKnight = new Knight($this, false);
        $bRightBishop = new Bishop($this, false);
        $this->cells[0][0] = $bLeftRook;
        $this->cells[0][1] = $bLeftKnight;
        $this->cells[0][2] = $bLeftBishop;
        $this->cells[0][3] = $bQueen;
        $this->cells[0][4] = $bKing;
        $this->cells[0][5] = $bRightBishop;
        $this->cells[0][6] = $bRightKnight;
        $this->cells[0][7] = $bRightRook;
        for ($i = 0; $i < self::CELL_WIDTH; $i++) {
            $this->cells[1][$i] = new Pawn($this, false);
        }
    }

    public function jsonSerialize()
    {
        $result = ["isWhiteNext" => $this->isWhiteNext, "cells" => []];

        for ($i = 0; $i < self::CELL_HEIGHT; $i++) {
            for ($j = 0; $j < self::CELL_WIDTH; $j++) {
                $piece = $this->cells[$i][$j];
                if ($piece !== null) {
                    array_push($result["cells"], $piece);
                }
            }
        }

        return $result;
    }
}

