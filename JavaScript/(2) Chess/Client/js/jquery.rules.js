var isValid = function($piece, $square) {
  return true;
}

var applyRules = function($piece) {
  switch ($piece.html()) {
    case '♔' || '♚':
      applyKingRules($piece);
      break;
    case '♕' || '♛':
      applyQueenRules($piece);
      break;
    case '♖' || '♜':
      applyRookRules($piece);
      break;
    case '♗' || '♝':
      applyBishopRules($piece);
      break;
    case '♘' || '♞':
      applyKnightRules($piece);
      break;
    case '♙' || '♟':
      applyPawnRules($piece);
      break;
  }
};

var applyKingRules = function($piece) {
  // TODO
};

var applyQueenRules = function($piece) {
  applyRookRules($piece);
  applyBishopRules($piece);
};

var _apply = function(index) {
  var $piece = getPiece(index);
  if ($piece.html() == undefined) {
    enableDroppable(getSquare(index));
    return true;
  }
  if (isEnemyPiece($piece)) {
    enableDroppable(getSquare(index));
    return false;
  }
  return false;
}

var applyRookRules = function($piece) {
  var squareIndex = $piece.parent().index();
  var rowIndex = Math.floor(squareIndex / 8);
  var columnIndex = squareIndex % 8;

  for (var offset = 1; rowIndex - offset >= 0 && _apply(squareIndex - offset * length); offset++);
  for (var offset = 1; rowIndex + offset < length && _apply(squareIndex + offset * length); offset++);
  for (var offset = 1; columnIndex - offset >= 0 && _apply(squareIndex - offset); offset++);
  for (var offset = 1; columnIndex + offset< length && _apply(squareIndex + offset); offset++);
};

var applyBishopRules = function($piece) {
  var squareIndex = $piece.parent().index();
  var rowIndex = Math.floor(squareIndex / 8);
  var columnIndex = squareIndex % 8;

  for (var offset = 1; rowIndex - offset >= 0 && columnIndex - offset >= 0 && _apply(squareIndex - offset * length - offset); offset++);
  for (var offset = 1; rowIndex - offset >= 0 && columnIndex + offset < length && _apply(squareIndex - offset * length + offset); offset++);
  for (var offset = 1; rowIndex + offset < length && columnIndex + offset < length && _apply(squareIndex + offset * length + offset); offset++);
  for (var offset = 1; rowIndex + offset < length && columnIndex - offset >= 0 && _apply(squareIndex + offset * length - offset); offset++);
};

var applyKnightRules = function($piece) {
  // TODO
};

var applyPawnRules = function($piece) {
  var squareIndex = $piece.parent().index();
  var rowIndex = Math.floor(squareIndex / 8);
  var columnIndex = squareIndex % 8;

  if (rowIndex == 0) {
    // TODO: Promotion.
    return;
  }

  var $oneForward = getPiece(squareIndex - length);
  var $twoForward = (rowIndex == length - 2) ? getPiece(squareIndex - 2 * length) : null;
  var $left = columnIndex ? getPiece(squareIndex - length - 1) : null;
  var $right = (columnIndex != length - 1) ? getPiece(squareIndex - length + 1) : null;

  if ($oneForward.html() != undefined) {
    $oneForward = $twoForward = null;
  }
  else if ($twoForward == null || $twoForward.html() != undefined) {
    $twoForward = null;
  }
  if ($left == null || $left.html() == undefined || isPlayerPiece($left)) {
    $left = null;
  }
  if ($right == null || $right.html() == undefined || isPlayerPiece($right)) {
    $right = null;
  }

  if ($oneForward != null) {
    enableDroppable(getSquare(squareIndex - length));
  }
  if ($twoForward != null) {
    enableDroppable(getSquare(squareIndex - 2 * length));
  }
  if ($left != null) {
    enableDroppable(getSquare(squareIndex - length - 1));
  }
  if ($right != null) {
    enableDroppable(getSquare(squareIndex - length + 1));
  }
};
