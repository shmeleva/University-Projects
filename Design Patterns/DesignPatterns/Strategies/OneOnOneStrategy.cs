using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DesignPatterns
{
    class OneOnOneStrategy : MilitaryStrategy
    {
        public override String Name
        {
            get
            {
                return "Один на одного";
            }
        }

        public OneOnOneStrategy(CommandManager commandManager, Battlefield field)
            : base(commandManager, field) { }


        public override void AttackPhase()
        {
            if (_field.Reds.Count == 0 || _field.Whites.Count == 0)
                throw new Exception("Army of the dead.");

            UnitKeeper firstTarget = _field.CurrentArmy[0].AttackTarget(_field.OppositeArmy[0]);
            firstTarget.Log.ObjectColor = _field.GetArmyName(_field.OppositeArmy);
            firstTarget.Log.SubjectColor = _field.GetArmyName(_field.CurrentArmy);
            ExecuteNewCommand(new FightCommand(firstTarget, _field));
            
            if (firstTarget.Changed.Health != 0)
            {
                UnitKeeper secondTarget = _field.OppositeArmy[0].AttackTarget(_field.CurrentArmy[0]);
                secondTarget.Log.ObjectColor = _field.GetArmyName(_field.CurrentArmy);
                secondTarget.Log.SubjectColor = _field.GetArmyName(_field.OppositeArmy);
                ExecuteNewCommand(new FightCommand(secondTarget, _field));
            }
        }

        public override void SpecialActionPhase()
        {
            RunSpecialActions(_field.CurrentArmy);
            RunSpecialActions(_field.OppositeArmy);
        }

        protected override void RunSpecialActions(List<Unit> army)
        {
            if (army.Count < 2)
                return;

            for (int i = 1; i < army.Count; i++)
            {
                ISpecialAction specialUnit = army[i] as ISpecialAction;

                // 1. Юнит обладает специальныйми способностями.
                if(specialUnit != null)
                {
                    // 2. Звезды говорят, что в этот раз юнит может использовать свои способности.
                    if (specialUnit.Probability > BattleRandom.Next(100))
                    {
                        // 3. Находим потенциальные цели.
                        List<Unit> targets = GetTargets(specialUnit, army, _field.GetOppositeArmy(army));

                        // 4. Если есть, из кого выбирать.
                        if (targets.Count != 0)
                        {
                            // 5. Выбираем того единственного.
                            Int32 targetIndex = BattleRandom.Next(targets.Count);

                            // 6. Выполняем специальное действие.
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
                Int32 enemyBorder = specialUnit.Range - specialUnitIndex;

                for (int i = 0; i < Math.Min(enemyBorder, enemies.Count); i++)
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
