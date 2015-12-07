using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DesignPatterns
{
    class CommandManager
    {
        private readonly Stack<Command> _done;
        private readonly Stack<Command> _undone;

        public Int32 MovesDone
        {
            get;
            private set;
        }
        public Int32 MovesUndone
        {
            get;
            private set;
        }

        public CommandManager(Battlefield field)
        {
            _done = new Stack<Command>();
            _undone = new Stack<Command>();
        }

        /// <summary>
        /// Выполняет новую команду, затирая отмененные.
        /// </summary>
        /// <param name="command">Command to execute.</param>
        public void ExecuteNewCommand(Command command)
        {
            if (command == null)
                return;

            _undone.Clear();
            MovesUndone = 0;

            command.Do();
            _done.Push(command);

            if (command is MoveEndCommand)
                MovesDone++;

            OnCommandEvent(command, CommandStatus.Done);
        }

        /// <summary>
        /// Отмееяет последний выполненных ход при его наличии.
        /// </summary>
        public void UndoMove()
        {
            if (MovesDone == 0)
                throw new Exception("There are no Moves to undo.");

            while (true)
            {
                var cmd = _done.Pop();
                cmd.Undo();
                _undone.Push(cmd);

                OnCommandEvent(cmd, CommandStatus.Undone);

                if (cmd is MoveStartCommand)
                    break;
            }

            MovesDone--;
            MovesUndone++;
        }

        /// <summary>
        /// Повторяет последний отменненный ход при его наличии.
        /// </summary>
        public void RedoMove()
        {
            if (MovesUndone == 0)
                throw new Exception("There are no Moves to redo.");

            while (true)
            {
                var cmd = _undone.Pop();
                cmd.Do();
                _done.Push(cmd);

                OnCommandEvent(cmd, CommandStatus.Redone);

                if (_undone.Count > 0 && _undone.Peek() is GameOverCommand)
                    continue;

                if (cmd is MoveEndCommand || cmd is GameOverCommand)
                    break;
            }

            MovesDone++;
            MovesUndone--;
        }

        /// <summary>
        /// Очищает историю выполненных и отмененных ходов.
        /// </summary>
        public void Clear()
        {
            _done.Clear();
            _undone.Clear();

            MovesDone = 0;
            MovesUndone = 0;
        }

        /// <summary>
        /// Отменяет все выполенные ходы и очищает историю. @ Проверить.
        /// </summary>
        public void Restore()
        {
            foreach (Command cmd in _done)
                cmd.Undo();

            Clear();
        }

        #region Observer

        public delegate void CommandEventHandler(Command cmd, CommandStatus status);
        public event CommandEventHandler CommandEvent;

        protected virtual void OnCommandEvent(Command cmd, CommandStatus status)
        {
            var handler = CommandEvent;
            if (handler != null)
                handler(cmd, status);
        }

        #endregion
    }
}
