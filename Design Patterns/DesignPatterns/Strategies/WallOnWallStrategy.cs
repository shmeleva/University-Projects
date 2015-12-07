using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DesignPatterns
{
    class WallOnWallStrategy : MilitaryStrategy
    {
        private List<Unit> _largerArmy;
        private Int32 _gap;

        public override String Name
        {
            get
            {
                return "Все на всех";
            }
        }

        public WallOnWallStrategy(CommandManager commandManager, Battlefield field)
            : base(commandManager, field) { }


        public override void AttackPhase()
        {
            if (_field.Reds.Count == 0 || _field.Whites.Count == 0)
                throw new Exception("Army of the dead.");

            List<Command> commandHistory = new List<Command>();

            _gap = _field.CurrentArmy.Count - _field.OppositeArmy.Count;

            if (_gap == 0)
            {
                _largerArmy = null;
            }
            else
            {
                _largerArmy = (_gap > 0) ? _field.CurrentArmy : _field.OppositeArmy;
                _gap = Math.Abs(_gap);
            }

            for (int i = 0; i < Math.Min(_field.CurrentArmy.Count, _field.OppositeArmy.Count); i++)
            {
                UnitKeeper firstTarget = _field.CurrentArmy[i].AttackTarget(_field.OppositeArmy[i]);
                firstTarget.Log.ObjectColor = _field.GetArmyName(_field.OppositeArmy);
                firstTarget.Log.SubjectColor = _field.GetArmyName(_field.CurrentArmy);
                commandHistory.Add(new FightCommand(firstTarget, _field));

                if (firstTarget.Changed.Health != 0)
                {
                    UnitKeeper secondTarget = _field.OppositeArmy[i].AttackTarget(_field.CurrentArmy[i]);
                    secondTarget.Log.ObjectColor = _field.GetArmyName(_field.CurrentArmy);
                    secondTarget.Log.SubjectColor = _field.GetArmyName(_field.OppositeArmy);
                    commandHistory.Add(new FightCommand(secondTarget, _field));
                }
            }

            foreach (Command command in commandHistory)
                ExecuteNewCommand(command);
        }

        public override void SpecialActionPhase()
        {
            if (_largerArmy != null)
                RunSpecialActions(_largerArmy);
        }

        protected override void RunSpecialActions(List<Unit> army)
        {
            for (int i = army.Count - _gap; i < army.Count; i++)
            {
                ISpecialAction specialUnit = army[i] as ISpecialAction;

                if (specialUnit != null)
                {
                    if (specialUnit.Probability > BattleRandom.Next(100))
                    {
                        List<Unit> targets = GetTargets(specialUnit, army, _field.GetOppositeArmy(army));

                        if (targets.Count != 0)
                        {
                            Int32 targetIndex = BattleRandom.Next(targets.Count);

                            UnitKeeper specialActionTarget = specialUnit.SpecialAction(targets[targetIndex]);
                            specialActionTarget.Log.ObjectColor = _field.GetArmyName(_field.FindAllies(targets[targetIndex]));
                            specialActionTarget.Log.SubjectColor = _field.GetArmyName(_field.FindAllies(specialUnit as Unit));
                            ExecuteNewCommand(new SpecialActionCommand(specialActionTarget, _field));
                        }
                    }
                }
            }
        }

        protected override List<Unit> GetTargets(ISpecialAction specialUnit, List<Unit> allies, List<Unit> enemies)
        {
            Int32 specialUnitIndex = allies.FindIndex(unit => unit == specialUnit as Unit), targetIndex;
            List<Unit> targets = new List<Unit>();

            if (specialUnit.Target == ActionTarget.Any || specialUnit.Target == ActionTarget.Ally)
            {
                if (specialUnit.IsCompatibleType(specialUnit as Unit))
                    targets.Add(specialUnit as Unit);

                for (int i = 1; i < specialUnit.Range + 1; i++)
                {
                    targetIndex = specialUnitIndex - i;

                    if (targetIndex < 0)
                        break;

                    if (specialUnit.IsCompatibleType(allies[targetIndex]))
                        targets.Add(allies[targetIndex]);
                }

                for (int i = 1; i < specialUnit.Range + 1; i++)
                {
                    targetIndex = specialUnitIndex + i;

                    if (targetIndex > allies.Count - 1)
                        break;

                    if (specialUnit.IsCompatibleType(allies[targetIndex]))
                        targets.Add(allies[targetIndex]);
                }
            }

            if (specialUnit.Target == ActionTarget.Any || specialUnit.Target == ActionTarget.Enemy)
            {
                Int32 startingIndex = specialUnitIndex - specialUnit.Range;
                startingIndex = startingIndex > 0 ? startingIndex : 0;

                Int32 endingIndex = specialUnitIndex + specialUnit.Range;

                for (int i = startingIndex; i < Math.Min(enemies.Count, endingIndex + 1); i++)
                {
                    if (specialUnit.IsCompatibleType(enemies[i]))
                    {
                        targets.Add(enemies[i]);
                    }
                }
            }

            return targets;
        }
    }
}
