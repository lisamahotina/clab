<?php
require_once $_SERVER['DOCUMENT_ROOT'] . "/Desk.php";
require_once $_SERVER['DOCUMENT_ROOT'] . "/helpers.php";


abstract class Piece implements JsonSerializable
{
    public $isWhite;
    public $desk;

    public function __construct($desk, $isWhite)
    {
        $this->desk = $desk;
        $this->isWhite = $isWhite;
    }
    // Определяет текущую клетку
    public function GetCurrentCell()
    {
        for ($i = 0; $i < Desk::CELL_HEIGHT; $i++) {
            for ($j = 0; $j < Desk::CELL_WIDTH; $j++) {
                if ($this->desk->cells[$i][$j] === $this)
                    return ['x' => $j, 'y' => $i];
            }
        }
        return ['x' => -1, 'y' => -1];
    }
    // Из всех возможных клеток определяет может ли фигура папасть в определенную клетку
    public function Move($newCell)
    {
        $curCell = $this->GetCurrentCell();
        if (!$this->OnDesk($curCell) || !$this->OnDesk($newCell)) return false;
        if (in_array($newCell, $this->GetAvailableCells($curCell))) {
            // Сохраняем фигуру соперника для того, чтобы проверить, будет ли король
            // после данного хода находится под боем. Если будет, надо вернуть фигуры на место
            $enemyPiece = $this->desk->cells[$newCell['y']][$newCell['x']];

            $this->desk->cells[$newCell['y']][$newCell['x']] = $this;
            $this->desk->cells[$curCell['y']][$curCell['x']] = null;

            if ($this->IsCheck()) {
                $this->desk->cells[$curCell['y']][$curCell['x']] = $this;
                $this->desk->cells[$newCell['y']][$newCell['x']] = $enemyPiece;
                return false;
            }
            return true;
        }
        return false;
    }

    // Если игрок находится в состоянии шаха
    // Если $isWhite = true, проверяем для белых
    // Если $isWhite = false, проверяем для черных
    function IsCheck($isWhite = null)
    {
        if ($isWhite === null) {
            $isWhite = $this->isWhite;
        }

        $king = null;
        $enemyAvailableCells = [];

        for ($y = 0; $y < Desk::CELL_HEIGHT; $y++) {
            for ($x = 0; $x < Desk::CELL_WIDTH; $x++) {
                $curPiece = $this->desk->cells[$y][$x];
                if ($curPiece === null)
                    continue;

                if ($isWhite === $curPiece->isWhite && $curPiece instanceof King)
                    $king = $curPiece;

                if ($isWhite !== $curPiece->isWhite) {
                    $curAvailableCells = $curPiece->GetAvailableCells($curPiece->GetCurrentCell());
                    foreach ($curAvailableCells as $curCell) {
                        if (!in_array($curCell, $enemyAvailableCells))
                            array_push($enemyAvailableCells, $curCell);
                    }
                }
            }
        }

        return in_array($king->GetCurrentCell(), $enemyAvailableCells);
    }

    // Если игрок находится в состоянии мата
    // Если $isWhite = true, проверяем для белых
    // Если $isWhite = false, проверяем для черных
    // Доделать
    function IsMate($isWhite)
    {
        ///if ($this->isCheck($this->isWhite)) {
            ///for ($y = 0; $y < Desk::CELL_HEIGHT; $y++) {
                ///for ($x = 0; $x < Desk::CELL_WIDTH; $x++) {
                    ///$suspected = $this->desk->cells[$y][$x];
                    ///if ($suspected === $this->isCheck($this->isWhite)) {
                         ///$saved = $suspected;
                         ///$curAvailableCells = $suspected->GetAvailableCells($suspected->GetCurrentCell());
                         ///foreach ($curAvailableCells as $piece) {
                             ///$suspected = $piece;
                             ///if ($suspected == !($this->isCheck($this->isWhite))) {
                                 ///return $saved;
    }

    abstract public function GetAvailableCells($curCell);

    /// Если возвращает true - идем в цикле дальше
    /// Если возвращает false - останавливаемся в цикле
    function AddToAvailableCells(&$availableCells, $cell)
    {
        $cellState = $this->GetCellState($cell);
        if ($cellState === Desk::OUT_OF_DESK ||
            $cellState === Desk::ME ||
            $cellState === Desk::FRIEND)
            return false;
        if ($cellState === Desk::ENEMY)
        {
            array_push($availableCells, $cell);
            return false;
        }

        array_push($availableCells, $cell);
        return true;
    }

    /// Если возвращает EMPTY_CELL - клетка пустая
    /// Если возвращает FRIEND - дружественная фигура
    /// Если возвращает ENEMY - противник
    /// Если возращает ME - текущая фигура
    function GetCellState($cell) {
        if (!$this->OnDesk($cell))
            return Desk::OUT_OF_DESK;

        $figure = $this->desk->cells[$cell['y']][$cell['x']];
        if ($figure === null) return Desk::EMPTY_CELL;
        if ($figure === $this) return Desk::ME;
        if ($figure->isWhite === $this->isWhite) return Desk::FRIEND;
        return Desk::ENEMY;
    }

    function OnDesk($cell) {
        if (
            $cell['x'] < 0 ||
            $cell['y'] < 0 ||
            $cell['x'] >= Desk::CELL_WIDTH ||
            $cell['y'] >= Desk::CELL_HEIGHT
        )
            return false;
        return true;
    }

    public function jsonSerialize()
    {
        $cell = $this->GetCurrentCell();
        return [
            "type" => get_class($this),
            "isWhite" => $this->isWhite,
            "cellName" => CoordsToCellName($cell),
            "x" => $cell["x"],
            "y" => $cell["y"]
        ];
    }
}

class King extends Piece
{
    public function __construct($desk, $isWhite)
    {
        parent::__construct($desk, $isWhite);
    }

    function AddToAvailableCells(&$availableCells, $cell)
    {
        // (добавить проверку, что рядом с клеткой нет вражеского короля)
        $cellState = $this->GetCellState($cell);

        // Если клетка пустая, либо там соперник, но не король
        if ($cellState === Desk::EMPTY_CELL || $cellState === Desk::ENEMY &&
            (!$this->desk->cells[$cell['y']][$cell['x']] instanceof King)) {
            array_push($availableCells, $cell);
        }
    }

    public function GetAvailableCells($curCell)
    {
        $result = [];

        $cell = ['x' => $curCell['x'] - 1, 'y' => $curCell['y'] - 1];
        $this->AddToAvailableCells($result, $cell);

        $cell = ['x' => $curCell['x'], 'y' => $curCell['y'] - 1];
        $this->AddToAvailableCells($result, $cell);

        $cell = ['x' => $curCell['x'] + 1, 'y' => $curCell['y'] - 1];
        $this->AddToAvailableCells($result, $cell);

        $cell = ['x' => $curCell['x'] + 1, 'y' => $curCell['y']];
        $this->AddToAvailableCells($result, $cell);

        $cell = ['x' => $curCell['x'] + 1, 'y' => $curCell['y'] + 1];
        $this->AddToAvailableCells($result, $cell);

        $cell = ['x' => $curCell['x'], 'y' => $curCell['y'] + 1];
        $this->AddToAvailableCells($result, $cell);

        $cell = ['x' => $curCell['x'] - 1, 'y' => $curCell['y'] + 1];
        $this->AddToAvailableCells($result, $cell);

        $cell = ['x' => $curCell['x'] - 1, 'y' => $curCell['y']];
        $this->AddToAvailableCells($result, $cell);

        return $result;
    }
}

class Rook extends Piece
{
    public function __construct($desk, $isWhite)
    {
        parent::__construct($desk, $isWhite);
    }

    public function GetAvailableCells($curCell)
    {
        $result = [];
        // Идем вверх
        for ($i = $curCell['y'] - 1; $i >= 0; $i--) {
            $cell = ['x' => $curCell['x'], 'y' => $i];
            $moveToNext = $this->AddToAvailableCells($result, $cell);
            if (!$moveToNext)
                break;
        }
        // Идем влево
        for ($j = $curCell['x'] - 1; $j >= 0; $j--) {
            $cell = ['x' => $j, 'y' => $curCell['y']];
            $moveToNext = $this->AddToAvailableCells($result, $cell);
            if (!$moveToNext)
                break;
        }
        // Идем вниз
        for ($i = $curCell['y'] + 1; $i < Desk::CELL_HEIGHT; $i++) {
            $cell = ['x' => $curCell['x'], 'y' => $i];
            $moveToNext = $this->AddToAvailableCells($result, $cell);
            if (!$moveToNext)
                break;
        }
        // Идем вправо
        for ($j = $curCell['x'] + 1; $j < Desk::CELL_WIDTH; $j++) {
            $cell = ['x' => $j, 'y' => $curCell['y']];
            $moveToNext = $this->AddToAvailableCells($result, $cell);
            if (!$moveToNext)
                break;
        }

        return $result;
    }
}

class Bishop extends Piece
{
    public function __construct($desk, $isWhite)
    {
        parent::__construct($desk, $isWhite);
    }

    public function GetAvailableCells($curCell)
    {
        $result = [];

        // Идем вверх влево
        $step = 1;
        while (true)
        {
            $cell = ['x' => $curCell['x'] - $step, 'y' => $curCell['y'] - $step];
            $moveToNext = $this->AddToAvailableCells($result, $cell);
            if (!$moveToNext)
                break;
            $step++;
        }
        // Идем вниз влево
        $step = 1;
        while (true)
        {
            $cell = ['x' => $curCell['x'] - $step, 'y' => $curCell['y'] + $step];
            $moveToNext = $this->AddToAvailableCells($result, $cell);
            if (!$moveToNext)
                break;
            $step++;
        }
        // Идем вниз вправо
        $step = 1;
        while (true)
        {
            $cell = ['x' => $curCell['x'] + $step, 'y' => $curCell['y'] + $step];
            $moveToNext = $this->AddToAvailableCells($result, $cell);
            if (!$moveToNext)
                break;
            $step++;
        }
        // Идем вверх вправо
        $step = 1;
        while (true)
        {
            $cell = ['x' => $curCell['x'] + $step, 'y' => $curCell['y'] - $step];
            $moveToNext = $this->AddToAvailableCells($result, $cell);
            if (!$moveToNext)
                break;
            $step++;
        }

        return $result;
    }
}

class Queen extends Piece
{
    public function __construct($desk, $isWhite)
    {
        parent::__construct($desk, $isWhite);
    }

    public function GetAvailableCells($curCell)
    {
        $tempRook = new Rook($this->desk, $this->isWhite);
        $tempBishop = new Bishop($this->desk, $this->isWhite);
        $result = array_merge($tempRook->GetAvailableCells($curCell), $tempBishop->GetAvailableCells($curCell));
        $tempBishop = null;
        $tempRook = null;
        return $result;
    }
}

class Knight extends Piece
{
    public function __construct($desk, $isWhite)
    {
        parent::__construct($desk, $isWhite);
    }

    public function GetAvailableCells($curCell)
    {
        $result = [];

        $cell = ['x' => $curCell['x'] - 2, 'y' => $curCell['y'] - 1];
        $this->AddToAvailableCells($result, $cell);
        $cell = ['x' => $curCell['x'] - 1, 'y' => $curCell['y'] - 2];
        $this->AddToAvailableCells($result, $cell);
        $cell = ['x' => $curCell['x'] - 2, 'y' => $curCell['y'] + 1];
        $this->AddToAvailableCells($result, $cell);
        $cell = ['x' => $curCell['x'] + 2, 'y' => $curCell['y'] - 1];
        $this->AddToAvailableCells($result, $cell);
        $cell = ['x' => $curCell['x'] - 1, 'y' => $curCell['y'] + 2];
        $this->AddToAvailableCells($result, $cell);
        $cell = ['x' => $curCell['x'] + 1, 'y' => $curCell['y'] - 2];
        $this->AddToAvailableCells($result, $cell);
        $cell = ['x' => $curCell['x'] + 2, 'y' => $curCell['y'] + 1];
        $this->AddToAvailableCells($result, $cell);
        $cell = ['x' => $curCell['x'] + 1, 'y' => $curCell['y'] + 2];
        $this->AddToAvailableCells($result, $cell);

        return $result;
    }
}

class Pawn extends Piece
{
    public function __construct($desk, $isWhite)
    {
        parent::__construct($desk, $isWhite);
    }

    public function GetAvailableCells($curCell)
    {
        $result = [];

        $newY = $this->isWhite ? $curCell['y'] - 1 : $curCell['y'] + 1;

        $cell = ['x' => $curCell['x'], 'y' => $newY];
        if ($this->GetCellState($cell) === Desk::EMPTY_CELL) {
            array_push($result, $cell);
        }

        $cell = ['x' => $curCell['x'] - 1, 'y' => $newY];
        if ($this->GetCellState($cell) === Desk::ENEMY) {
            array_push($result, $cell);
        }

        $cell = ['x' => $curCell['x'] + 1, 'y' => $newY];
        if ($this->GetCellState($cell) === Desk::ENEMY) {
            array_push($result, $cell);
        }

        // Если пешка стоит на начальной позиции, она может пойти через одну клетку

        // Для белых
        if ($this->isWhite && $curCell['y'] === Desk::CELL_HEIGHT - 2) {
            $cell2 = ['x' => $curCell['x'], 'y' => $curCell['y'] - 2];
            if ($this->GetCellState($cell) === Desk::EMPTY_CELL &&
                $this->GetCellState($cell2) === Desk::EMPTY_CELL)
                array_push($result, $cell2);
        }

        // Для черных
        if (!$this->isWhite && $curCell['y'] === 1) {
            $cell2 = ['x' => $curCell['x'], 'y' => $curCell['y'] + 2];
            if ($this->GetCellState($cell) === Desk::EMPTY_CELL &&
                $this->GetCellState($cell2) === Desk::EMPTY_CELL)
                array_push($result, $cell2);
        }

        return $result;
    }
}
