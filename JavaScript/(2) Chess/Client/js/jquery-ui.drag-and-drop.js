var enableDragAndDrop = function() {
  $('.draggable-piece').draggable({
    revert: 'invalid',
    disabled: true,

    start: function(event, ui) {
      $(this).addClass('grabbed');
      applyRules($(this));
    },

    stop: function(event, ui) {
      $(this).removeClass('grabbed');
    }
  });

  $('.droppable-square').droppable({
    accept: '.draggable-piece',
    hoverClass: 'drop-enabled-hover',
    disabled: true,

    drop: function(event, ui) {
      var $square = $(this);
      var $piece = ui.draggable;

      if ($piece.parent().position().top != $square.position().top || $piece.parent().position().left != $square.position().left) {
        clear($square);
        move($piece, $square);
      }

      align($piece, $square);

      $('#chessboard').children('.droppable-square').each(function() {
        disableDroppable($(this));
      });
    }
  });
};

var enableDroppable = function($droppableSquare) {
  $droppableSquare.droppable("option", "disabled", false);
};

var disableDroppable = function($droppableSquare) {
  $droppableSquare.droppable("option", "disabled", true);
};

var enableDraggable = function($draggablePiece) {
  $draggablePiece.draggable("option", "disabled", false);
  $draggablePiece.addClass('drag-enabled');
};

var disableDraggable = function($draggablePiece) {
  $draggablePiece.draggable("option", "disabled", true);
  $draggablePiece.removeClass('drag-enabled');
};

var align = function($piece, $square) {
  $piece.offset({ top: $square.position().top, left: $square.position().left });
};

var move = function($piece, $square) {
  $piece.detach().appendTo($square);
};

var clear = function($square) {
  $square.children('.draggable-piece').first().detach();
};
