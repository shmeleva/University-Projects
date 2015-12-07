using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace DesignPatterns
{
    class Battle
    {
        private readonly CommandManager _commandManager;

        private readonly Battlefield _field;

        private readonly List<MilitaryStrategy> _availableStrategies;
        private MilitaryStrategy _currentStrategy;

        private readonly BattleInterruptController _interrupt;

        public Battlefield Field
        {
            get
            {
                return _field;
            }
        }
        public List<MilitaryStrategy> AvailableStrategies
        {
            get
            {
                return _availableStrategies;
            }
        }
        public MilitaryStrategy CurrentStrategy
        {
            get
            {
                return _currentStrategy;
            }
        }


        public Int32 MovesDone
        {
            get
            {
                return _commandManager.MovesDone;
            }
        }
        public Int32 MovesUndone
        {
            get
            {
                return _commandManager.MovesUndone;
            }
        }

        public Battle()
		{
            _field = new Battlefield();
            _field.Status = Battlefield.BattleStatus.Active;

            _commandManager = new CommandManager(_field);
			_commandManager.CommandEvent += HandleCommand;

            _availableStrategies = new List<MilitaryStrategy>(3);
            LoadAvailableStrategies();
            if (_availableStrategies.Count == 0)
                throw new Exception("No available strategies.");
            
            _currentStrategy = _availableStrategies[0];

            _interrupt = new BattleInterruptController(_field, 10);
		}

        public void ChangeStrategy(Int32 index)
        {
            _currentStrategy = _availableStrategies[index];
        }

        /// <summary>
        /// Следующий ход.
        /// </summary>
        public void Action()
        {
            if (_field.Status != Battlefield.BattleStatus.Active)
                throw new Exception("This battle is over");

            _commandManager.ExecuteNewCommand(new MoveStartCommand());
            _currentStrategy.AttackPhase();
            _currentStrategy.SpecialActionPhase();
            _commandManager.ExecuteNewCommand(new MoveEndCommand(_field));

            InterruptIfNecessary();
        }

        /// <summary>
        /// Ход назад.
        /// </summary>
        public void Back()
        {
            _commandManager.UndoMove();
        }

        /// <summary>
        /// Ход вперед.
        /// </summary>
        public void Forward()
        {
            _commandManager.RedoMove();
        }

        /// <summary>
        /// Восстановление начального состояния поля боя.
        /// </summary>
        public void Restore()
        {
            _commandManager.Restore();
        }

        /// <summary>
        /// Загрузка доступных стратегий боя.
        /// </summary>
        private void LoadAvailableStrategies()
        {
            if (_availableStrategies == null)
                return;

            foreach (Type t in Assembly.GetExecutingAssembly().GetTypes())
            {
                if (t.BaseType == typeof(MilitaryStrategy))
                {
                    var ctor = t.GetConstructor(new[] { typeof(CommandManager), typeof(Battlefield) });

                    if (ctor != null)
                    {
                        MilitaryStrategy temp = ctor.Invoke(new object[] { _commandManager, _field }) as MilitaryStrategy;

                        if (temp != null)
                        {
                            _availableStrategies.Add(temp);
                        }
                    }
                }
            }
        }

        /// <summary>
        /// Прерывает бой в случае смерти одной из армий или затянувшийся бой.
        /// </summary>
        private void InterruptIfNecessary()
        {
            _interrupt.Update();
            if (_interrupt.UnitsAreBored)
            {
                _commandManager.ExecuteNewCommand(new GameOverCommand(_field, "Units are bored."));
            }

            if (_field.Reds.Count == 0)
            {
                if (_field.Whites.Count == 0)
                {
                    _commandManager.ExecuteNewCommand(new GameOverCommand(_field, "Drawn battle."));
                }
                else
                {
                    _commandManager.ExecuteNewCommand(new GameOverCommand(_field, String.Format("{0} won the victory.", _field.GetArmyName(_field.Whites))));
                }
            }
            else if (_field.Whites.Count == 0)
            {
                _commandManager.ExecuteNewCommand(new GameOverCommand(_field, String.Format("{0} won the victory.", _field.GetArmyName(_field.Reds))));
            }
         }

        #region Observer

        private void HandleCommand(Command cmd, CommandStatus status)
        {
            OnLogEvent(string.Format("{0}: {1}", status, cmd)); 

            ActionCommand actionCmd = cmd as ActionCommand;

            if (cmd.Status != CommandStatus.Undone)
            {
                if (actionCmd != null)
                {
                    if (actionCmd.IsDeadly)
                    {
                        OnDeathEvent(actionCmd.Target.Changed);
                    }

                    if (actionCmd is SpecialActionCommand)
                    {
                        OnSpecialActionEvent(actionCmd.ToString());
                    }

                    return;
                }

                GameOverCommand overCmd = cmd as GameOverCommand;

                if (overCmd != null)
                {
                    OnGameOverEvent(overCmd.ToString());
                }
            }
        }

        // Запись в лог.
        public delegate void LogEventHandler(String message);
        public event LogEventHandler LogEvent;

        private void OnLogEvent(String message)
        {
            var handler = LogEvent;

            if (handler != null)
                handler(message);
        }

        // Смерть юнита.
        public delegate void DeathEventHandler(Unit unit);
        public event DeathEventHandler DeathEvent;

        private void OnDeathEvent(Unit unit)
        {
            var handler = DeathEvent;

            if (handler != null)
                handler(unit);
        }

        // Специальное действие.
        public delegate void SpecialActionEventHandler(String message);
        public event SpecialActionEventHandler SpecialActionEvent;

        private void OnSpecialActionEvent(String message)
        {
            var handler = SpecialActionEvent;

            if (handler != null)
                handler(message);
        }

        // Завершение боя.
        public delegate void GameOverEventHandler(String message);
        public event GameOverEventHandler GameOverEvent;

        private void OnGameOverEvent(String message)
        {
            var handler = GameOverEvent;

            if (handler != null)
                handler(message);
        }

        #endregion
    }
}
