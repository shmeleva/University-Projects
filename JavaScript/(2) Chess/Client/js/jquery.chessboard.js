var initialized = false;
var reversed = false;

var length = 8;
var files = ['A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'];
var ranks = [8, 7, 6, 5, 4, 3, 2, 1];
var unicodeBoard =
	['♜', '♞', '♝', '♛', '♚', '♝', '♞', '♜',
	'♟', '♟', '♟', '♟', '♟', '♟', '♟', '♟',
	null, null, null, null, null, null, null, null,
	null, null, null, null, null, null, null, null,
	null, null, null, null, null, null, null, null,
	null, null, null, null, null, null, null, null,
	'♙', '♙', '♙', '♙', '♙', '♙', '♙', '♙',
	'♖', '♘', '♗', '♕', '♔', '♗', '♘', '♖'];

var initializeChessboard = function(reversed) {
	if (initialized) {
		return;
	}

	if (reversed) {
		reverseBoard();
	}

	isPlayerPiece = reversed ? isBlackPiece : isWhitePiece;
	isEnemyPiece = reversed ? isWhitePiece : isBlackPiece;

	for (var i = 0; i < ranks.length; i++) {
		for (var j = 0; j < files.length; j++) {
			var count = files.length * i + j;

			var style = (count % files.length) ? null : ' style="clear: left"';
			var coordinates = '<div class = "coordinate">' + files[j] + ranks[i] + '</div>'
			var colorClass = getSquareColor(i, j);
			var $currentsquare = $('<div class="droppable-square ' + colorClass + '"' + style + '">' + coordinates + '</div>');
			$("#chessboard").append($currentsquare);

			if (unicodeBoard[count] != null) {
				var $piece = $('<div class = "draggable-piece">' + unicodeBoard[count] + '</div>');
				$currentsquare.append($piece);
			}
		}
	}

	initialized = true;
	console.log('Chessboard initialized');

};

var enablePlayer = function() {
	$('#chessboard').find('.draggable-piece').each(function() {
		$piece = $(this);
		if (isPlayerPiece($piece)) {
			enableDraggable($piece);
		}
	});
};

var disablePlayer = function() {
	$('#chessboard').find('.draggable-piece').each(function() {
		$piece = $(this);
		if (isPlayerPiece($piece)) {
			disablePlayer($piece);
		}
	});
};

var isBlackPiece = function($piece) {
	var piece = $piece.html();
	return (piece >= '♚') && (piece <= '♟');
};

var isWhitePiece = function($piece) {
	var piece = $piece.html();
	return (piece >= '♔') && (piece <= '♙');
};

var isPlayerPiece;
var isEnemyPiece;

var reverseBoard = function() {
	files.reverse();
	ranks.reverse();
	unicodeBoard.reverse();

	reversed = !reversed;
};

var getSquareColor = function(i, j) {
	return ( !(i % 2) && (j % 2) || (i % 2) && !(j % 2) ) ? 'black-square' : 'white-square';
};

var getSquare = function(index) {
  return $('#chessboard').children('.droppable-square').eq(index);
}

var getPiece = function(squareIndex) {
  return $('#chessboard').children('.droppable-square').eq(squareIndex).children('.draggable-piece').first();
};
