using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DesignPatterns
{
    class MoveEndCommand : Command
    {
        private Battlefield _field;
        private List<Unit> _oldCurrentArmy;

        public MoveEndCommand(Battlefield field) : base()
        {
            _field = field;
        }

        protected override void _Do()
        {
            _oldCurrentArmy = _field.CurrentArmy;
            _field.SetCurrentArmy(_field.GetOppositeArmy(_oldCurrentArmy));
        }

        protected override void _Undo()
        {
            _field.SetCurrentArmy(_oldCurrentArmy);
        }

        public override string ToString()
        {
            return "Конец этапа боя.";
        }
    }
}
