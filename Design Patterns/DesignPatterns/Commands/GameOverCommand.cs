using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DesignPatterns
{
    class GameOverCommand : Command
    {
        private Battlefield _field;
        private String _message;

        public GameOverCommand(Battlefield field, String message) : base()
        {
            _field = field;
            _message = message;
        }

        protected override void _Do()
        {
            _field.Status = Battlefield.BattleStatus.Finished;
        }

        protected override void _Undo()
        {
            _field.Status = Battlefield.BattleStatus.Active;
        }

        public override string ToString()
        {
            return "This battle if over. " + _message;
        }
    }
}
