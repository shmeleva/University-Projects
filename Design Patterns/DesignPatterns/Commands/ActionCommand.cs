using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DesignPatterns
{
    /// <summary>
    /// Хранит информацию о совершенной атаке или специальном действии
    /// и вносит или отменяет соответствующие изменения в армиях.
    /// </summary>
    class ActionCommand : Command
    {
        private readonly UnitKeeper _target;
        private readonly List<Unit> _allies;
        private Int32 _index;
        private readonly Boolean _isDeadly;

        public Boolean IsDeadly
        {
            get
            {
                return _isDeadly;
            }
        }

        public UnitKeeper Target
        {
            get
            {
                return _target;
            }
        }

        public List<Unit> Allies
        {
            get
            {
                return _allies;
            }
        }

        public ActionCommand(UnitKeeper target, Battlefield field) : base()
        {
            _target = target;

            _isDeadly = (_target.Changed != null && _target.Changed.Health == 0) ? true : false;

            _allies = field.FindAllies(_target.Original);
            if (_allies == null)
                throw new InvalidOperationException("Allies were not found.");

            _index = _allies.FindIndex(original => original == _target.Original);
        }

        protected override void _Do()
        {
            _index = _allies.FindIndex(original => original == _target.Original);

            if (Target.Changed != null)
            {
                Allies.Remove(Target.Original);

                if (Target.Changed.Health != 0)
                {
                    Allies.Insert(_index, Target.Changed);
                }
            }

            if (Target.Created != null)
            {
                Allies.Add(Target.Created);
            }
        }

        protected override void _Undo()
        {
            if (Target.Changed != null)
            {
                if (Target.Changed.Health != 0)
                {
                    Allies.Remove(Target.Changed);
                }

                Allies.Insert(_index, Target.Original);
            }

            if (Target.Created != null)
            {
                Allies.Remove(Target.Created);
            }
        }

        public override string ToString()
        {
            return Target.Log.ToString();
        }
    }
}
