using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DesignPatterns
{
    abstract class Command
    {
        private CommandStatus _commandStatus;

        public CommandStatus Status
        {
            get
            {
                return _commandStatus;
            }
        }

        protected abstract void _Do();

        protected abstract void _Undo();

        protected Command()
        {
            _commandStatus = CommandStatus.Ready;
        }

        public void Do()
        {
            if (Status == CommandStatus.Done || Status == CommandStatus.Redone)
                throw new InvalidOperationException("The action has been done already.");

            _Do();

            _commandStatus = (_commandStatus == CommandStatus.Ready) ? CommandStatus.Done : CommandStatus.Redone;
        }

        public void Undo()
        {
            if (Status == CommandStatus.Ready || Status == CommandStatus.Undone)
                throw new InvalidOperationException("The action needs to be done first.");

            _Undo();

            _commandStatus = CommandStatus.Undone;
        }
    }
}
